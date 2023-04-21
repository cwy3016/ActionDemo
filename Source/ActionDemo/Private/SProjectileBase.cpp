// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	MovmentComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovmentComp");
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");

	// ÉèÖÃÅö×²ÎÄ¼þ
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp->SetupAttachment(SphereComp);

	MovmentComp->InitialSpeed = 1000.f;
	MovmentComp->bRotationFollowsVelocity = true;
	MovmentComp->bInitialVelocityInLocalSpace = true;

	Damage = -10.f;
}

void ASProjectileBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator()) {
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp) {
			AttributeComp->ApplyHealthChange(Damage);
		}
	}
	
	Explode();
}

void ASProjectileBase::Explode_Implementation()
{
	// spawn emitter cannot be pending kill state
	if (ensure(!IsPendingKill())) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeVFX, GetActorLocation(), GetActorRotation());
	}
	Destroy();
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &ASProjectileBase::OnHit);

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

