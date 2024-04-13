/**
* @file EOSSession.cpp
 * 
 * @brief Author: Marcel Gheorghe Becheanu
 * @brief Last Updated: April 12, 2024
 * @brief Github: https://github.com/marcelbecheanu
 * 
 */

#include "OnlineSessionSettings.h"
#include "EOSStrategyCore.h"
#include "EOSSession.h"

#include "Interfaces/OnlineSessionInterface.h"

void UEOSSession::Initialize(UEOSStrategyCore* EOSStrategyCore)
{
	EOSStrategyCorePtr = EOSStrategyCore;
	checkf(EOSStrategyCorePtr != nullptr, TEXT("Failed to initialize EOSStrategyCore in EOSSession!"));
}

void UEOSSession::CreateOnlineSession(FSessionInfo SessionInfo)
{
	if(!EOSStrategyCorePtr->HasOnlineSubsystem())
	{
		const FString ErrorMessage = "Online subsystem not available.";
        UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
        if (OnCreateOnlineSessionCompletedDelegate.IsBound()) {
            OnCreateOnlineSessionCompletedDelegate.Broadcast(false, ErrorMessage);
        }
		return;
	}


	//SessionInfoPtr = &SessionInfo;
	
	if(!SessionInfo.ConnectionSettings.bIsDedicated)
	{
		if(!EOSStrategyCorePtr->HasOnlineIdentity())
		{
			const FString ErrorMessage = "Online Identity not available.";
			UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
			if (OnCreateOnlineSessionCompletedDelegate.IsBound()) {
				OnCreateOnlineSessionCompletedDelegate.Broadcast(false, ErrorMessage);
			}
			return;
		}

		if(!EOSStrategyCorePtr->GetAuthenticator()->IsAuthenticated())
		{
			const FString ErrorMessage = "Player is not authenticated.";
			UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
			if (OnCreateOnlineSessionCompletedDelegate.IsBound()) {
				OnCreateOnlineSessionCompletedDelegate.Broadcast(false, ErrorMessage);
			}
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

	// TODO: ATENÇÃO PARA ALTERAR
	SessionCreationInfo.BuildUniqueId = SessionInfo.ConnectionSettings.BuildUniqueId;

	//EOSStrategyCorePtr->GetOnlineSession()->GetSessionSettings(FName(SessionInfo.SessionName))->Set(FName("WorldName"), FString(SessionInfo.WorldName),  EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	EOSStrategyCorePtr->GetOnlineSession()->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSSession::OnCreateOnlineSessionCompleted);
	EOSStrategyCorePtr->GetOnlineSession()->CreateSession(0, FName(SessionInfo.SessionName), SessionCreationInfo);
}

void UEOSSession::OnCreateOnlineSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	EOSStrategyCorePtr->GetOnlineSession()->ClearOnCreateSessionCompleteDelegates(this);
	if(!bWasSuccessful)
	{
		const FString ErrorMessage = "GENERATE MESSAGE";
		UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
		if (OnCreateOnlineSessionCompletedDelegate.IsBound()) {
			OnCreateOnlineSessionCompletedDelegate.Broadcast(bWasSuccessful, *ErrorMessage);
		}
		return;
	}

	GetWorld()->ServerTravel(FString(SessionInfoPtr->WorldPath + "?listen?port=7770"));
	if (OnCreateOnlineSessionCompletedDelegate.IsBound()) {
		OnCreateOnlineSessionCompletedDelegate.Broadcast(true, FString("Server has Started!"));
	}
}
