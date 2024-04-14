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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFindOnlineSessionCompletedDelegate, bool, bWasSuccessful, FString, Error);

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
	FString SessionName = FString("");
	
	/** The name of the world associated with this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FString WorldName = FString("");

	/** The path to the world associated with this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FString WorldPath = FString("");

	/** The port number for the server associated with this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	int32 PortServer = 3000;

	/** The connection settings for this session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	FConnectionSettings ConnectionSettings;
};

USTRUCT(BlueprintType)
struct FSearchSettings
{
	GENERATED_BODY()
	
public:
	
	/** Whether the query is intended for LAN matches or not */
    UPROPERTY(BlueprintReadWrite, Category = "Search Settings")
    bool bIsLanQuery = false;

    /** Max number of queries returned by the matchmaking service */
    UPROPERTY(BlueprintReadWrite, Category = "Search Settings")
    int32 MaxSearchResults = 1000000;

    /** Used to sort games into buckets since a the difference in terms of feel for ping in the same bucket is often not a useful comparison and skill is better */
    UPROPERTY(BlueprintReadWrite, Category = "Search Settings")
    int32 PingBucketSize = 0;

    /** Search hash used by the online subsystem to disambiguate search queries, stamped every time FindSession is called */
    UPROPERTY(BlueprintReadWrite, Category = "Search Settings")
    int32 PlatformHash = 0;

    /** Amount of time to wait for the search results. May not apply to all platforms. */
    UPROPERTY(BlueprintReadWrite, Category = "Search Settings")
    float TimeoutInSeconds;

};

UCLASS()
class EOSSTRATEGY_API UEOSSession : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initializes the authenticator with the EOS strategy core.
	 * 
	 * @param EOSStrategyCore The EOS strategy core.
	 */
	void Initialize(UEOSStrategyCore* EOSStrategyCore);

	UFUNCTION(BlueprintCallable, Category= "EOS|Session|Action")
	void CreateOnlineSession(FSessionInfo SessionInfo);
	
	/**
	* @brief Event dispatcher for Create Online Session completion.
	*/
	UPROPERTY(BlueprintAssignable, Category = "EOS|Session|Event")
	FOnCreateOnlineSessionCompletedDelegate OnCreateOnlineSessionCompletedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "EOS|Session|Event")
	FOnFindOnlineSessionCompletedDelegate OnFindOnlineSessionCompletedDelegate;

	UFUNCTION(BlueprintCallable, Category = "EOS|Session|Query")
	void FindOnlineSessions(FSearchSettings SearchSettings);

private:
	// Pointer to the EOS strategy core
	UEOSStrategyCore* EOSStrategyCorePtr;

	// Session Info store info about created session
	FSessionInfo SessionInfoPtr;

	// Used to Storage Sessions
	TSharedPtr<class FOnlineSessionSearch> OnlineSessionSearch;

	void OnCreateOnlineSessionCompleted(FName SessionName, bool bWasSuccessful);
	void HandleSessionCreationFailure(const FString& ErrorMessage) const;

	void OnFindOnlineSessionsCompleted(bool bWasSuccess);
	void HandleFindOnlineSessionsFailure(const FString& ErrorMessage) const;
};
