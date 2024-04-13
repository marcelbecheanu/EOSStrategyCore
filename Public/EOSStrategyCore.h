/**
 * @file EOSStrategyCore.h
 * 
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 11, 2024
 * @brief Github: https://github.com/marcelbecheanu
 * 
 */

#pragma once

#include "EOSAuthenticator.h"
#include "OnlineSubsystem.h"
#include "CoreMinimal.h"
#include "EOSSession.h"
#include "Engine/GameInstance.h"
#include "EOSStrategyCore.generated.h"

/**
 * @brief The main class for managing EOS-related functionalities.
 */
UCLASS()
class EOSSTRATEGY_API UEOSStrategyCore : public UGameInstance
{
    GENERATED_BODY()

public:
   // Reference to the EOS Authenticator Module.
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS|Authenticator")
   UEOSAuthenticator* Authenticator;

   /**
    * @brief Retrieves the EOS Authenticator Module.
    * 
    * @return A pointer to the EOS Authenticator Module.
    */
    UFUNCTION(BlueprintCallable,Category = "EOS|Authenticator|Query")
    UEOSAuthenticator* GetAuthenticator();

    // Reference to the EOS Authenticator Module.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EOS|Session")
    UEOSSession* Session;

    /**
     * @brief Retrieves the EOS Authenticator Module.
     * 
     * @return A pointer to the EOS Authenticator Module.
     */
    UFUNCTION(BlueprintCallable,Category = "EOS|Session|Query")
    UEOSSession* GetSession();
 
  
    /**
     * @brief Initializes the EOS strategy core.
     * 
     * This function initializes the EOS subsystem and obtains the EOS identity interface.
     */
    virtual void Init() override;

    /**
     * @brief Checks if the EOS subsystem is initialized.
     * 
     * @return true if the EOS subsystem is initialized, false otherwise.
     */
    bool HasOnlineSubsystem() const;

    /**
     * @brief Retrieves the EOS subsystem.
     * 
     * @return A pointer to the EOS subsystem.
     */
    IOnlineSubsystem* GetOnlineSubsystem() const;

    /**
     * @brief Checks if the EOS identity interface is available.
     * 
     * @return true if the EOS identity interface is available, false otherwise.
     */
    bool HasOnlineIdentity() const;

    /**
     * @brief Retrieves the EOS identity interface.
     * 
     * @return A pointer to the EOS identity interface.
     */
    IOnlineIdentityPtr GetOnlineIdentity() const;

    /**
     * @brief Checks if the EOS session interface is available.
     * 
     * @return true if the EOS session interface is available, false otherwise.
     */
    bool HasOnlineSession() const;

    /**
     * @brief Retrieves the EOS session interface.
     * 
     * @return A pointer to the EOS session interface.
     */
    IOnlineSessionPtr GetOnlineSession() const;

protected:

private:
    // Reference to the online subsystem.
    IOnlineSubsystem* OnlineSubsystem = nullptr;

    // Reference to the online identity interface.
    IOnlineIdentityPtr OnlineIdentity = nullptr;

    // Reference to the online session interface.
    IOnlineSessionPtr OnlineSession = nullptr;
    
};
