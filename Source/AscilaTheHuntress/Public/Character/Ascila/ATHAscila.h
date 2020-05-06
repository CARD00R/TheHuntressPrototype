// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ATHAscila.generated.h"

class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class EParentStance : uint8
{
	Eps_Standing UMETA(DisplayName = "Standing"),
	Eps_Crouching UMETA(DisplayName = "Crouching"),
	Eps_Rolling UMETA(DisplayName = "Rolling"),
	Eps_InAir UMETA(DisplayName = "InAir"),
	Eps_Dead UMETA(DisplayName = "Dead"),
	Eps_Max UMETA(DisplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class EStanceStatus : uint8
{
	Ess_StandIdling UMETA(DisplayName = "StandIdling"),
	Ess_StandJogging UMETA(DisplayName = "StandJogging"),
	Ess_StandSprinting UMETA(DisplayName = "StandSprinting"),
	Ess_CrouchIdling UMETA(DisplayName = "CrouchIdling"),
	Ess_CrouchWalking UMETA(DisplayName = "CrouchWalking"),
	Ess_CrouchSprinting UMETA(DisplayName = "CrouchSprinting"),
	Ess_InAir UMETA(DisplayName = "InAir"),
	Ess_NA UMETA(DisplayName = "NA"),
	Ess_Max UMETA(DisplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class ERequestStance : uint8
{
	Ers_StandIdling UMETA(DisplayName = "StandIdling"),
	Ers_StandJogging UMETA(DisplayName = "StandJogging"),
	Ers_StandSprinting UMETA(DisplayName = "StandSprinting"),
	Ers_CrouchIdling UMETA(DisplayName = "CrouchIdling"),
	Ers_CrouchWalking UMETA(DisplayName = "CrouchWalking"),
	Ers_CrouchSprinting UMETA(DisplayName = "CrouchSprinting"),
	Ers_NA UMETA(DisplayName = "NA"),
	Ers_Max UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class ASCILATHEHUNTRESS_API AATHAscila : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AATHAscila();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera")
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera")
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCharacterMovementComponent* CharMovementComp;

	//Parent Stances
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "States")
	EParentStance ParentStance;
	void SetParentStanceStatus(EParentStance Status);
	EParentStance GetParentStanceStatus();
	
	// Stance Status
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "States")
	EStanceStatus StanceStatus;
	void SetStanceStatus(EStanceStatus Status);
	EStanceStatus GetStanceStatus();
	
	// Request Stance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "States")
	ERequestStance RequestStance;
	void SetRequestedStatus(ERequestStance RequestedStance);
	ERequestStance GetRequestedStance();

	void SetYaw(float NewYaw);
	void SetPitch(float NewPitch);
	
protected:
	// Called when the game starts or	when spawned
	virtual void BeginPlay() override;

	// Initialisation Variables
		// Mesh Component
	FVector TargetMeshLocation = FVector(0, 0, 0);
			// Stand
	FVector StandMeshInitialiseLocation = FVector(-9, -4.5, -95.0);
	FRotator StandMeshInitialiseRotation = FRotator(0, -90, 0);
			// Crouch
	FVector CrouchMeshInitialiseLocation = FVector(-5, -4.5, -72);
	FVector CrouchSprintMeshInitialiseLocation = FVector(-20, 0, -76);
			// Capsule Component
	float StandCapsuleRadius = 24.0f;
	float StandCapsuleHalfHeight = 93;
	float CrouchCapsuleRadius = 30.0f;
	float CrouchCapsuleHalfHeight = 70.0f;
	float CrouchSprintCapsuleRadius = 30.0f;
	float CrouchSprintCapsuleHalfHeight = 75.0f;
	float TargetCapsuleRadius = 0.0f;
	float TargetCapsuleHalfHeight = 0.0f;
				// Capsule Speed Alpha changes
	float HeightAlpha = 0.02f;
	float RadiusAlpha = 0.02f;
	float LocationAlpha = 0.02f;
	UPROPERTY(EditAnywhere, Category = "Components|Capsule Properties")
	float CapsuleMeshAlpha = 0.01f;

	// Input
	bool bIsMovingForward = false;
	bool bIsMovingRight = false;
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void LookRight(float value);

	// Movement
		// Character Movement Speed
	void SetCharacterSpeed(float Speed);
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float JogSpeed = 520.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float SprintSpeed = 950.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float CrouchWalkSpeed = 175.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float CrouchSprintSpeed = 600.0f;
		// Idle
	UPROPERTY(VisibleInstanceOnly, Category = "Movement Properties")
	bool bIdleCheck = true;
	void IdleCheck();
		// Sprint
	void RequestSprint();
	void Sprint();
	void SprintReleased();
		// Crouch
	void RequestCrouchChange();
	void AscilaCrouch();
	void AscilaUnCrouch();
	void CapsuleMeshProprtiesChange();
	FTimerHandle CapsuleMeshProprtiesChangeTimer;

	// Weapons
	float Pitch;
	float Yaw;
	void RequestDrawChange();
	void DrawBow();
	void UnDrawBow();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
