// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATHBow.generated.h"

//class UBoxComponent;
class AATHArrow;

UCLASS()
class ASCILATHEHUNTRESS_API AATHBow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATHBow();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	UBoxComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// InitialisationVariables
	FTransform ArrowTransform;
	FRotator BowStartRotation = FRotator(90.0f, 180.0f, 180.0f);
	FVector BowStartLocation = FVector(-37, 0, 0);
	FVector BowSize = FVector(1.3f, 1.3f, 1.3f);

	// Fire
	void SpawnArrow();
	
	// Arrow Type
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties|Arrow")
	TSubclassOf<AActor> ArrowClass;
	AATHArrow* EquippedArrowType;
	void CheckArrowParameters();
	float ArrowBaseDamage = 0.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Fire
	void FireArrow();
};
