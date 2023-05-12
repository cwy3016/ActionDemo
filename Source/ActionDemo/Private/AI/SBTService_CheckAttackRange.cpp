// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// check the distance between ai pawn and target actor
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComponent)) {
		if (AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"))) {
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController)) {
				APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn)) {
					float Distance = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, Distance < 800.f);
				}
			}
		}
	}
}
