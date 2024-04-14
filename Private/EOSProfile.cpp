// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSProfile.h"

#include "EOSStrategyCore.h"
#include "Interfaces/OnlineIdentityInterface.h"

void UEOSProfile::Initialize(UEOSStrategyCore* EOSStrategyCore)
{
    EOSStrategyCorePtr = EOSStrategyCore;
    checkf(EOSStrategyCorePtr != nullptr, TEXT("Failed to initialize EOSStrategyCore in EOSAuthenticator!"));
}

FString UEOSProfile::GetPlayerNickname() const
{
    if (!EOSStrategyCorePtr->HasOnlineSubsystem() || !EOSStrategyCorePtr->HasOnlineIdentity() || !EOSStrategyCorePtr->GetAuthenticator()->IsAuthenticated())
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot get player nickname. Not authenticated."));
        return FString();
    }

    return EOSStrategyCorePtr->GetOnlineIdentity()->GetPlayerNickname(0);
}