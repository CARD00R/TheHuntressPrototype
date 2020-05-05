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
	Ess_SIdling UMETA(DisplayName = "SIdling"),
	Ess_SWalking UMETA(DisplayName = "SWalking"),
	Ess_SJogging UMETA(DisplayName = "SJogging"),
	Ess_SSprinting UMETA(DisplayName = "SSprinting"),
	Ess_CIdling UMETA(DisplayName = "CIdling"),
	Ess_CWalking UMETA(DisplayName = "CWalking"),
	Ess_InAir UMETA(DisplayName = "InAir"),
	Ess_NA UMETA(DisplayName = "NA"),
	Ess_Max UMETA(DisplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class ERequestStance : uint8
{
	Ers_SIdling UMETA(DisplayName = "SIdling"),
	Ers_SWalking UMETA(DisplayName = "SWalking"),
	Ers_SJogging UMETA(DisplayName = "SJogging"),
	Ers_SSprinting UMETA(DisplayName = "SSprinting"),
	Ers_CIdling UMETA(DisplayName = "CIdling"),
	Ers_CWalking UMETA(DisplayName = "CWalking"),
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCharacterMovementComponent* CharMovementComp;

	//Parent Stances
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	EParentStance ParentStance;
	void SetParentStanceStatus(EParentStance Status);
	EParentStance GetParentStanceStatus();
	
	// Stance Status
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	EStanceStatus StanceStatus;
	void SetStanceStatus(EStanceStatus Status);
	EStanceStatus GetStanceStatus();
	
	// Request Stance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	ERequestStance RequestStance;
	void SetRequestedStatus(ERequestStance RequestedStance);
	ERequestStance GetRequestedStance();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialisation Variables
		// Mesh Component
	FVector MeshInitialiseLocation = FVector(0, 0, -102.0);
	FRotator MeshInitialiseRotation = FRotator(0, -90, 0);
		// Capsule Component
	float CapsuleRadius = 20.0f;
	float CapsuleHalfHeight = 100;

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
	float WalkSpeed = 145.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float JogSpeed = 520.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float SprintSpeed = 900.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float CrouchSpeed = 175.0f;
		// Idle
	bool bIdleCheck = true;
	void IdleCheck();
		// Walk
	void RequestWalk();
	void Walk();
	void WalkReleased();
		// Sprint
	void RequestSprint();
	void Sprint();
	void SprintReleased();
		// Crouch
	void RequestCrouch();
	void AscilaCrouch();
	void AscilaCrouchRelease();
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
