// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATHArrow.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class ASCILATHEHUNTRESS_API AATHArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATHArrow();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* HitCollision;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USkeletalMeshComponent* MeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// InitialisationVariables
	FRotator ArrowStartRotation = FRotator(90.0f, 180.0f, 180.0f);
	FVector ArrowStartLocation = FVector(-37, 0, 0);
	FVector ArrowSize = FVector(1.3f, 1.3f, 1.3f);
	
	// Rotate Arrow based on Velocity
	void ArrowRotation();
	FTimerHandle ArrowRotationHandle;

	// Damage
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage = 20.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetPowerVelocity(float PowerVel);

	float GetDamage();
};
