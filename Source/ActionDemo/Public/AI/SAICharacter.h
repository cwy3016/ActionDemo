// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class ACTIONDEMO_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();
	virtual void PostInitializeComponents() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* HitInstigator, USAttributeComponent* OwningComp, float Health, float Delta);

	void SetTargetActor(AActor* NewTarget);
};
