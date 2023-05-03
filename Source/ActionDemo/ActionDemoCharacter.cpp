// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionDemoCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInteractionComponent.h"
#include "SAttributeComponent.h"


//////////////////////////////////////////////////////////////////////////
// AActionDemoCharacter

AActionDemoCharacter::AActionDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// create a interactionComponent
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	// create a attributeComponent
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
}

void AActionDemoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AActionDemoCharacter::OnHealthChanged);
}

void AActionDemoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AActionDemoCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AActionDemoCharacter::PrimaryAttack_TimeElapsed, 0.3f);
}

void AActionDemoCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void AActionDemoCharacter::DashAttack()
{
	PlayAnimMontage(AttackMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AActionDemoCharacter::DashAttack_TimeElapsed, 0.3f);
}

void AActionDemoCharacter::DashAttack_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void AActionDemoCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackMontage);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AActionDemoCharacter::BlackHoleAttack_TimeElapsed, 0.3f);
}

void AActionDemoCharacter::BlackHoleAttack_TimeElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void AActionDemoCharacter::PrimaryInteract()
{
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

void AActionDemoCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (!ensure(ClassToSpawn)) return;

	FVector HandLocation = GetMesh()->GetSocketLocation("weapon_l");

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	/* make spawn transform */
	// setup collision query params;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(20.f);

	FCollisionObjectQueryParams CollisionObjParams;
	CollisionObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	CollisionObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	CollisionObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// check hit result of trace 
	FVector TraceStart = FollowCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 5000);
	FHitResult Hit;
	if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, CollisionObjParams, CollisionShape, CollisionParams)) {
		TraceEnd = Hit.ImpactPoint;
	}
	
	FTransform SpawnTM = FTransform(FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator() , HandLocation);
	GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
}

void AActionDemoCharacter::OnHealthChanged(AActor* HitInstigator, USAttributeComponent* OwningComp, float Health, float Delta)
{
	if (Health <= 0.f) {
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AActionDemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AActionDemoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AActionDemoCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AActionDemoCharacter::PrimaryAttack);

		// DashShooting
		EnhancedInputComponent->BindAction(DashShootAction, ETriggerEvent::Triggered, this, &AActionDemoCharacter::DashAttack);

		// Interact
		EnhancedInputComponent->BindAction(BlackHoleShootAction, ETriggerEvent::Triggered, this, &AActionDemoCharacter::BlackHoleAttack);

		// Interact
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AActionDemoCharacter::PrimaryInteract);
	}

}

void AActionDemoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AActionDemoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




