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

	// load collision file named projectile
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	EffectComp->SetupAttachment(SphereComp);

	MovmentComp->InitialSpeed = 1000.f;
	MovmentComp->bRotationFollowsVelocity = true;
	MovmentComp->bInitialVelocityInLocalSpace = true;

	Damage = -50.f;
}

void ASProjectileBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator()) {
		if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()))) {
			AttributeComp->ApplyHealthChange(GetInstigator(), Damage);
		}
		Explode();
	}
}

void ASProjectileBase::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator()) {
		if (USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()))) {
			AttributeComp->ApplyHealthChange(GetInstigator(), Damage);
		}
		Explode();
	}
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
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectileBase::OnActorOverlap);
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

