/**
 * @file EOSAuthenticator.cpp
 *
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 11, 2024
 * @brief Github: https://github.com/marcelbecheanu
 *
 * This file contains the implementation of the UEOSAuthenticator class, which handles user authentication with EOS.
 */

#include "EOSAuthenticator.h"
#include "EOSStrategyCore.h"
#include "Interfaces/OnlineIdentityInterface.h"

// Initialize method to set the EOS strategy core
void UEOSAuthenticator::Initialize(UEOSStrategyCore* EOSStrategyCore)
{
    EOSStrategyCorePtr = EOSStrategyCore;
    checkf(EOSStrategyCorePtr != nullptr, TEXT("Failed to initialize EOSStrategyCore in EOSAuthenticator!"));
}

// Authenticate method to authenticate a user with EOS
void UEOSAuthenticator::Authenticate(const FString& UserID, const FString& UserToken, const FString& LoginType)
{
    // Check if EOS subsystem and identity interface are available
    if (!EOSStrategyCorePtr->HasOnlineSubsystem() || !EOSStrategyCorePtr->HasOnlineIdentity()) {
        // If not available, broadcast authentication failure
        if (OnAuthenticationCompleted.IsBound()) {
            OnAuthenticationCompleted.Broadcast(false, FString("Failed to authenticate. Online subsystem or identity interface not available."));
        }
        return;
    }

    // Create account credentials
    FOnlineAccountCredentials AccountCredentials;
    AccountCredentials.Id = UserID;
    AccountCredentials.Token = UserToken;
    AccountCredentials.Type = LoginType;

    // Add delegate for login completion
    EOSStrategyCorePtr->GetOnlineIdentity()->OnLoginCompleteDelegates->AddUObject(this, &UEOSAuthenticator::OnAuthenticateCompleted);

    // Initiate login
    EOSStrategyCorePtr->GetOnlineIdentity()->Login(0, AccountCredentials);
}

// Callback function for login completion
void UEOSAuthenticator::OnAuthenticateCompleted(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
    // Remove delegate
    EOSStrategyCorePtr->GetOnlineIdentity()->ClearOnLoginCompleteDelegates(0, this);

    // Broadcast authentication completion event
    if (OnAuthenticationCompleted.IsBound()) {
        OnAuthenticationCompleted.Broadcast(bWasSuccess, *Error);
    }

    // Log success or failure
    if (bWasSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Login successful"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Login failed. Reason: %s"), *Error);
    }
}

// Checks if the player is authenticated.
bool UEOSAuthenticator::IsAuthenticated() const {
    // Check if EOS subsystem and identity interface are available
    if(!EOSStrategyCorePtr->HasOnlineSubsystem() || !EOSStrategyCorePtr->HasOnlineIdentity())
        return false;

    // Check the login status
    return EOSStrategyCorePtr->GetOnlineIdentity()->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}
