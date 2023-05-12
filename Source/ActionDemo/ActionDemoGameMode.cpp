// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionDemoGameMode.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "SAttributeComponent.h"

AActionDemoGameMode::AActionDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	SpawnTimeInterval = 2.f;
	MaxBotNum = 10;
}

void AActionDemoGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBot, this, &AActionDemoGameMode::SpawnBotTimerElapsed, SpawnTimeInterval, true);
}

void AActionDemoGameMode::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* EnvQueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(EnvQueryInstance)) {
		EnvQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &AActionDemoGameMode::OnSpawnBotQueryCompleted);
	}
}

void AActionDemoGameMode::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		return;
	}

	// num should less than max bot number
	int32 BotAliveNum = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		if (USAttributeComponent::IsActorAlive(*It)) {
			++BotAliveNum;
		}
	}

	if (DifficultyCurve) {
		MaxBotNum = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if (BotAliveNum >= MaxBotNum) {
		return;
	}
	
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.Num() > 0) {
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}
