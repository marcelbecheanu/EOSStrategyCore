/**
 * @file EOSStrategyCore.cpp
 * 
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 11, 2024
 * @brief Github: https://github.com/marcelbecheanu
 *
 */

#include "EOSStrategyCore.h"
#include "EOSAuthenticator.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UEOSAuthenticator* UEOSStrategyCore::GetAuthenticator()
{
	return Authenticator;
}

UEOSSession* UEOSStrategyCore::GetSession()
{
	return Session;
}

// This function initializes the EOS subsystem and obtains the EOS identity interface.
void UEOSStrategyCore::Init()
{
	Super::Init();

	// Initialize the EOS subsystem
	OnlineSubsystem = Online::GetSubsystem(this->GetWorld());
	checkf(OnlineSubsystem != nullptr, TEXT("Failed to initialize OnlineSubsystem! Please ensure that OnlineSubsystem is properly configured."));

	// Obtain the EOS identity interface
	OnlineIdentity = OnlineSubsystem->GetIdentityInterface();
	checkf(OnlineIdentity != nullptr, TEXT("Failed to obtain OnlineIdentityInterface!"));

	// Obtain the EOS Session interface
	OnlineSession = OnlineSubsystem->GetSessionInterface();
	checkf(OnlineSession != nullptr, TEXT("Failed to obtain OnlineSessionInterface!"));

	// Obtain the EOS Authenticator Handler
	Authenticator = NewObject<UEOSAuthenticator>();
	checkf(Authenticator != nullptr, TEXT("Failed to initialize EOSAuthenticator Handler!"));
	Authenticator->Initialize(this);

	// Obtain the EOS Session Handler
	Session = NewObject<UEOSSession>();
	checkf(Session != nullptr, TEXT("Failed to initialize EOSSession Handler!"));
	Session->Initialize(this);
}


// Checks if the EOS subsystem is initialized.
bool UEOSStrategyCore::HasOnlineSubsystem() const
{
	return OnlineSubsystem != nullptr;
}

// Retrieves the EOS subsystem.
IOnlineSubsystem* UEOSStrategyCore::GetOnlineSubsystem() const
{
	return OnlineSubsystem;
}

// Checks if the EOS identity interface is available.
bool UEOSStrategyCore::HasOnlineIdentity() const
{
	return OnlineIdentity != nullptr;
}

// Retrieves the EOS identity interface.
IOnlineIdentityPtr UEOSStrategyCore::GetOnlineIdentity() const
{
	return OnlineIdentity;
}

// Checks if the EOS session interface is available.
bool UEOSStrategyCore::HasOnlineSession() const
{
	return OnlineSession != nullptr;
}

// Retrieves the EOS session interface.
IOnlineSessionPtr UEOSStrategyCore::GetOnlineSession() const
{
	return OnlineSession;
}
