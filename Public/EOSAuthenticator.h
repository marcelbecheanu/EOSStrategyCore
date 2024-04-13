/**
 * @file EOSAuthenticator.h
 * 
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 11, 2024
 * @brief Github: https://github.com/marcelbecheanu
 * 
 * This file contains the declaration of the UEOSAuthenticator class, which handles user authentication with EOS.
 */

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EOSAuthenticator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAuthenticationCompletedDelegate, bool, bWasSuccessful, FString, Error);
class UEOSStrategyCore;

/**
 * @brief Handles user authentication with EOS (Epic Online Services).
 */
UCLASS()
class EOSSTRATEGY_API UEOSAuthenticator : public UObject
{
    GENERATED_BODY()

public:
    /**
     * @brief Initializes the authenticator with the EOS strategy core.
     * 
     * @param EOSStrategyCore The EOS strategy core to be used for authentication.
     */
    void Initialize(UEOSStrategyCore* EOSStrategyCore);

    /**
     * @brief Event dispatcher for authentication completion.
     */
    UPROPERTY(BlueprintAssignable, Category = "EOS|Authenticator|Event")
    FOnAuthenticationCompletedDelegate OnAuthenticationCompleted;

    /**
     * @brief Authenticates a user with EOS using the provided user ID, token, and login type.
     * 
     * @param UserID The user's ID.
     * @param UserToken The user's authentication token.
     * @param LoginType The type of login.
     */
    UFUNCTION(BlueprintCallable, Category = "EOS|Authenticator|Action")
    void Authenticate(const FString& UserID, const FString& UserToken, const FString& LoginType);

    /**
     * @brief Checks if the player is authenticated.
     * 
     * @return True if authenticated, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "EOS|Authenticator|Query")
    bool IsAuthenticated() const;
private:
    // Pointer to the EOS strategy core
    UEOSStrategyCore* EOSStrategyCorePtr;

    /**
     * @brief Callback function for login completion.
     * 
     * @param LocalUserNum The local user number.
     * @param bWasSuccess Indicates whether the login was successful.
     * @param UserId The unique identifier for the user.
     * @param Error Any error message associated with the login attempt.
     */
    void OnAuthenticateCompleted(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
};
