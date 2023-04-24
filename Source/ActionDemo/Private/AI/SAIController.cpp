// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0)) {
		GetBlackboardComponent()->SetValueAsVector("LocationMoveTo", PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	}
}
