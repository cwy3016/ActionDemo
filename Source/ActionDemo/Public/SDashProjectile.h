// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONDEMO_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	FTimerHandle TimerHandle_DetonateDelay;

	virtual void BeginPlay() override;
	virtual void Explode_Implementation() override;
	void TeleportInstigator();

public:
	ASDashProjectile();
};
