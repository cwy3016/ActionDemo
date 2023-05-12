// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ActionDemoGameMode.generated.h"

class UEnvQueryInstanceBlueprintWrapper;

UCLASS(minimalapi)
class AActionDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AActionDemoGameMode();
	
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="AI")
	float SpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TSubclassOf<AActor>MinionClass;

	FTimerHandle TimerHandle_SpawnBot;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};



