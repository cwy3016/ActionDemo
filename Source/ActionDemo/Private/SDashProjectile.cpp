// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ASDashProjectile::ASDashProjectile()
{
	DetonateDelay = 0.2f;
	TeleportDelay = 0.2f;

	MovmentComp->InitialSpeed = 6000.f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_DetonateDelay, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	// maybe called by OnActorHit event;
	GetWorldTimerManager().ClearTimer(TimerHandle_DetonateDelay);
	
	// spawn emitter manually, auto destroyed
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeVFX, GetActorLocation(), GetActorRotation(), true);
	
	EffectComp->DeactivateSystem();

	MovmentComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	// teleport the instigator
	FTimerHandle TimeHandle_Teleport;
	GetWorldTimerManager().SetTimer(TimeHandle_Teleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (IsValid(ActorToTeleport)) {
		// teleport to where projectile is and keep rotation of the instigator
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
	Destroy();
}
