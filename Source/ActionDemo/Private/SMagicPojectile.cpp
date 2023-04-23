// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicPojectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASMagicPojectile::ASMagicPojectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	MovmentComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovmentComp");
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");

	// load collision file named projectile
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp->SetupAttachment(SphereComp);

	MovmentComp->InitialSpeed = 1000.f;
	MovmentComp->bRotationFollowsVelocity = true;
	MovmentComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ASMagicPojectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASMagicPojectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

