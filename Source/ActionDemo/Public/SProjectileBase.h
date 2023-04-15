// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;
class UPrimitiveComponent;

UCLASS(ABSTRACT)
class ACTIONDEMO_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// base properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UProjectileMovementComponent* MovmentComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UParticleSystemComponent* EffectComp;

	// visual effect when explode
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* ExplodeVFX;
	
	// probably called by on component hit event
	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	// _implement is necessary
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();
};
