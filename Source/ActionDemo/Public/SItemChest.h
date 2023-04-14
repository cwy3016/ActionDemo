// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONDEMO_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	// 接口实现
	void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	
	// 宝箱底座
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	// 宝箱盖子
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ASItemChest();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
