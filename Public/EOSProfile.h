// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EOSProfile.generated.h"

class UEOSStrategyCore;

UCLASS()
class EOSSTRATEGY_API UEOSProfile : public UObject
{
 GENERATED_BODY()
    
public:
    /**
     * @brief Initializes the authenticator with the EOS strategy core.
     *
     * @param EOSStrategyCore The EOS strategy core
     */
    void Initialize(UEOSStrategyCore* EOSStrategyCore);

    UFUNCTION(BlueprintCallable, Category = "EOS|Profile|Query")
    FString GetPlayerNickname() const;

private:
    // Pointer to the EOS strategy core
    UEOSStrategyCore* EOSStrategyCorePtr;
};
