// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100.f;
	Health = MaxHealth;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor) {
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* FromActor)
{
	if (USAttributeComponent* AttributeComp = GetAttributes(FromActor)) {
		return AttributeComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::ApplyHealthChange(AActor* HitInstigator, float DeltaHealth)
{
	
	Health += DeltaHealth;
	Health = Health <= 0 ? 0 : Health;
	Health = Health >= MaxHealth ? MaxHealth : Health;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("%s health is: %f"), *(GetOwner()->GetName()), Health));

	OnHealthChanged.Broadcast(HitInstigator, this, Health, DeltaHealth);
	return true;
}

bool USAttributeComponent::IsAlive()
{
	return Health > 0.f;
}
