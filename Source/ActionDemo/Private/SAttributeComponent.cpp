// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}


bool USAttributeComponent::ApplyHealthChange(float DeltaHealth)
{
	
	Health += DeltaHealth;
	Health = Health <= 0 ? 0 : Health;
	Health = Health >= MaxHealth ? MaxHealth : Health;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Player %s health is: %f"), *(GetOwner()->GetName()), Health));

	OnHealthChanged.Broadcast(nullptr, this, Health, DeltaHealth);
	return true;
}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.f;
}
