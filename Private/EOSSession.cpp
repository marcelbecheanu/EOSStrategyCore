/**
* @file EOSSession.cpp
 * 
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 12, 2024
 * @brief Github: https://github.com/marcelbecheanu
 * 
 */

#include "EOSSession.h"
#include "OnlineSessionSettings.h"
#include "EOSStrategyCore.h"

#include "Interfaces/OnlineSessionInterface.h"

void UEOSSession::Initialize(UEOSStrategyCore* EOSStrategyCore)
{
	EOSStrategyCorePtr = EOSStrategyCore;
	checkf(EOSStrategyCorePtr != nullptr, TEXT("Failed to initialize EOSStrategyCore in EOSSession!"));
}

void UEOSSession::CreateOnlineSession(FSessionInfo SessionInfo)
{
	SessionInfoPtr = SessionInfo;

	if (!EOSStrategyCorePtr->HasOnlineSubsystem())
	{
		HandleSessionCreationFailure("Online subsystem not available.");
		return;
	}
	if (!EOSStrategyCorePtr->HasOnlineSession())
	{
		HandleSessionCreationFailure("Online Session is not available.");
		return;
	}
	if (!SessionInfo.ConnectionSettings.bIsDedicated)
	{
		if (!EOSStrategyCorePtr->HasOnlineIdentity())
		{
			HandleSessionCreationFailure("Online Identity not available.");
			return;
		}

		if (!EOSStrategyCorePtr->GetAuthenticator()->IsAuthenticated())
		{
			HandleSessionCreationFailure("Player is not authenticated.");
			return;
		}
	}

	FOnlineSessionSettings SessionCreationInfo;
	SessionCreationInfo.bAllowInvites = SessionInfo.ConnectionSettings.bAllowInvites;
	SessionCreationInfo.bAllowJoinInProgress = SessionInfo.ConnectionSettings.bAllowJoinInProgress;
	SessionCreationInfo.bAllowJoinViaPresence = SessionInfo.ConnectionSettings.bAllowJoinViaPresence;
	SessionCreationInfo.bAllowJoinViaPresenceFriendsOnly = SessionInfo.ConnectionSettings.bAllowJoinViaPresenceFriendsOnly;
	SessionCreationInfo.bAntiCheatProtected = SessionInfo.ConnectionSettings.bAntiCheatProtected;
	SessionCreationInfo.bIsDedicated = SessionInfo.ConnectionSettings.bIsDedicated;
	SessionCreationInfo.bIsLANMatch = SessionInfo.ConnectionSettings.bIsLANMatch;
	SessionCreationInfo.bShouldAdvertise = SessionInfo.ConnectionSettings.bShouldAdvertise;
	SessionCreationInfo.bUseLobbiesIfAvailable = SessionInfo.ConnectionSettings.bUseLobbiesIfAvailable;
	SessionCreationInfo.bUsesPresence = SessionInfo.ConnectionSettings.bUsesPresence;
	SessionCreationInfo.bUsesStats = SessionInfo.ConnectionSettings.bUsesStats;
	SessionCreationInfo.NumPrivateConnections = SessionInfo.ConnectionSettings.NumPrivateConnections;
	SessionCreationInfo.NumPublicConnections = SessionInfo.ConnectionSettings.NumPublicConnections;
	SessionCreationInfo.BuildUniqueId = SessionInfo.ConnectionSettings.BuildUniqueId;

	// TODO: ATENÇÃO PARA ALTERAR
	// EOSStrategyCorePtr->GetOnlineSession()->GetSessionSettings(FName(SessionInfo.SessionName))->Set(FName("WorldName"), FString(SessionInfo.WorldName),  EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionCreationInfo.Settings.Add(FName("WORLD"), FOnlineSessionSetting((FString("TESTE")), EOnlineDataAdvertisementType::ViaOnlineService));
	EOSStrategyCorePtr->GetOnlineSession()->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSSession::OnCreateOnlineSessionCompleted);
	EOSStrategyCorePtr->GetOnlineSession()->CreateSession(0, FName(SessionInfo.SessionName), SessionCreationInfo);
}
void UEOSSession::OnCreateOnlineSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	EOSStrategyCorePtr->GetOnlineSession()->ClearOnCreateSessionCompleteDelegates(this);
	if (!bWasSuccessful)
	{
		HandleSessionCreationFailure("Failed to create online session.");
		return;
	}

	bool bHasHosted = EOSStrategyCorePtr->GetWorld()->ServerTravel(FString(SessionInfoPtr.WorldPath + "?listen?port=" + FString::FromInt(SessionInfoPtr.PortServer)));
	if (OnCreateOnlineSessionCompletedDelegate.IsBound())
	{
		OnCreateOnlineSessionCompletedDelegate.Broadcast(bHasHosted, bHasHosted ? "Server has Started!" : "Failed to create online session. Check your internet connection and try again later.");
	}
}
void UEOSSession::HandleSessionCreationFailure(const FString& ErrorMessage) const
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	if (OnCreateOnlineSessionCompletedDelegate.IsBound())
	{
		OnCreateOnlineSessionCompletedDelegate.Broadcast(false, ErrorMessage);
	}
}

void UEOSSession::FindOnlineSessions(FSearchSettings SearchSettings)
{
	if (!EOSStrategyCorePtr->HasOnlineSubsystem())
	{
		HandleFindOnlineSessionsFailure("Online subsystem not available.");
		return;
	}

	if (!EOSStrategyCorePtr->HasOnlineSession())
	{
		HandleFindOnlineSessionsFailure("Online Session is not available.");
		return;
	}
	
	if (!EOSStrategyCorePtr->GetAuthenticator()->IsAuthenticated())
	{
		HandleFindOnlineSessionsFailure("Player authentication failed. Please log in to your account.");
		return;
	}

	OnlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
	OnlineSessionSearch->bIsLanQuery = SearchSettings.bIsLanQuery;
	OnlineSessionSearch->PingBucketSize = SearchSettings.PingBucketSize;
	OnlineSessionSearch->MaxSearchResults = SearchSettings.MaxSearchResults;
	OnlineSessionSearch->TimeoutInSeconds = SearchSettings.TimeoutInSeconds;
	OnlineSessionSearch->PlatformHash = SearchSettings.PlatformHash;
	OnlineSessionSearch->QuerySettings.SearchParams.Empty();
	EOSStrategyCorePtr->GetOnlineSession()->FindSessions(0, OnlineSessionSearch.ToSharedRef());
	EOSStrategyCorePtr->GetOnlineSession()->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSSession::OnFindOnlineSessionsCompleted);
}

void UEOSSession::OnFindOnlineSessionsCompleted(bool bWasSuccess)
{
	EOSStrategyCorePtr->GetOnlineSession()->ClearOnFindSessionsCompleteDelegates(this);
	UE_LOG(LogTemp, Warning, TEXT("Online Session Search Completed: %s"), bWasSuccess ? TEXT("Success") : TEXT("Failed"));
	UE_LOG(LogTemp, Warning, TEXT("Number of Sessions Found: %d"), OnlineSessionSearch->SearchResults.Num());

	if(!bWasSuccess)
	{
		HandleFindOnlineSessionsFailure("GENERATE MESSAGE");
		return;
	}
	
}

void UEOSSession::HandleFindOnlineSessionsFailure(const FString& ErrorMessage) const
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
	if (OnFindOnlineSessionCompletedDelegate.IsBound())
	{
		OnFindOnlineSessionCompletedDelegate.Broadcast(false, ErrorMessage);
	}
}

