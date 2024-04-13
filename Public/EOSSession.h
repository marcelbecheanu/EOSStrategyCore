/**
* @file EOSSession.h
 * 
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 12, 2024
 * @brief Github: https://github.com/marcelbecheanu
 * 
 */
#pragma once

#include "CoreMinimal.h"
#include "EOSSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateOnlineSessionCompletedDelegate, bool, bWasSuccessful, FString, Error);
class UEOSStrategyCore;
USTRUCT(BlueprintType)
struct FConnectionSettings
{
	GENERATED_BODY()

public:
    /** Whether the match allows invitations for this session or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bAllowInvites = true;

    /** Whether joining in progress is allowed or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bAllowJoinInProgress = true;

    /** Whether joining via player presence is allowed or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bAllowJoinViaPresence = true;

    /** Whether joining via player presence is allowed for friends only or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bAllowJoinViaPresenceFriendsOnly = true;

    /** Whether the server employs anti-cheat (punkbuster, vac, etc). */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bAntiCheatProtected = false;

    /** Whether the server is dedicated or player hosted. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bIsDedicated = false;

    /** This game will be LAN only and not be visible to external players. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bIsLANMatch = false;

    /** Whether this match is publicly advertised on the online service. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bShouldAdvertise = true;

    /** Used to keep different builds from seeing each other during searches. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    int32 BuildUniqueId = 0;

    /** Whether to prefer lobbies APIs if the platform supports them. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bUseLobbiesIfAvailable = true;

    /** Whether to display user presence information or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bUsesPresence = true;

    /** Whether the match should gather stats or not. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    bool bUsesStats = true;


    /** The number of connections that are private (invite/password) only. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    int32 NumPrivateConnections = 0;

    /** The number of publicly available connections advertised. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
    int32 NumPublicConnections = 0;
};

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()
	
public:
	/** The name session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FString SessionName;
	
	/** The name of the world associated with this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FString WorldName;

	/** The path to the world associated with this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FString WorldPath;

	/** The port number for the server associated with this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	int32 PortServer = 3000;

	/** The connection settings for this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FConnectionSettings ConnectionSettings;
};

UCLASS()
class EOSSTRATEGY_API UEOSSession : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initializes the authenticator with the EOS strategy core.
	 * 
	 * @param EOSStrategyCore The EOS strategy core to be used for authentication.
	 */
	void Initialize(UEOSStrategyCore* EOSStrategyCore);

	UFUNCTION(BlueprintCallable, Category= "EOS|Session|Action")
	void CreateOnlineSession(FSessionInfo SessionInfo);
	
	/**
	* @brief Event dispatcher for Create Online Session completion.
	*/
	UPROPERTY(BlueprintAssignable, Category = "EOS|Session|Event")
	FOnCreateOnlineSessionCompletedDelegate OnCreateOnlineSessionCompletedDelegate;
	
private:
	// Pointer to the EOS strategy core
	UEOSStrategyCore* EOSStrategyCorePtr;

	FSessionInfo* SessionInfoPtr = nullptr;

	void OnCreateOnlineSessionCompleted(FName SessionName, bool bWasSuccessful);

};
