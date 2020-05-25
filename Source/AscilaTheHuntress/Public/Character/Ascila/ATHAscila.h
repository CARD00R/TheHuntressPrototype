// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ATHAscila.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AATHAscilaPC;
class AATHBow;
class UArrowComponent;

UENUM(BlueprintType)
enum class EParentStance : uint8
{
	Eps_Standing UMETA(DisplayName = "Standing"),
	Eps_Crouching UMETA(DisplayName = "Crouching"),
	Eps_Rolling UMETA(DisplayName = "Rolling"),
	Eps_InAir UMETA(DisplayName = "InAir"),
	Eps_Parkouring UMETA(DisplayName = "Parkouring"),
	Eps_Dead UMETA(DisplayName = "Dead"),
	Eps_Max UMETA(DisplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class EStanceStatus : uint8
{
	Ess_StandIdling UMETA(DisplayName = "StandIdling"),
	Ess_StandWalking UMETA(DisplayName = "StandWalking"),
	Ess_StandJogging UMETA(DisplayName = "StandJogging"),
	Ess_StandSprinting UMETA(DisplayName = "StandSprinting"),
	Ess_StandIdleJumping UMETA(DisplayName = "StandIdleJumping"),
	Ess_StandJogJumping UMETA(DisplayName = "StandJogJumping"),
	Ess_StandSprintJumping UMETA(DisplayName = "StandSprintJumping"),
	Ess_CrouchIdling UMETA(DisplayName = "CrouchIdling"),
	Ess_CrouchWalking UMETA(DisplayName = "CrouchWalking"),
	Ess_CrouchSprinting UMETA(DisplayName = "CrouchSprinting"),
	Ess_InAirJogFalling UMETA(DisplayName = "InAirJogFalling"),
	Ess_InAirSprintFalling UMETA(DisplayName = "InAirSprintFalling"),
	Ess_LandRolling UMETA(DisplayName = "LandRolling"),
	Ess_LandHard UMETA(DisplayName = "LandHard"),
	Ess_NA UMETA(DisplayName = "NA"),
	Ess_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EParkourStatus : uint8
{
	Eps_NA UMETA(DisplayName = "NA"),
	Eps_BracedIdling UMETA(DisplayName = "BracedIdling"),
	Eps_BracedMovingRight UMETA(DisplayName = "BracedMovingRight"),
	Eps_BracedMovingLeft UMETA(DisplayName = "BracedMovingLeft"),
	//Eps_BracedLeaningRight UMETA(DisplayName = "BracedLeaningRight"),
	//Eps_BracedLeaningLeft UMETA(DisplayName = "BracedLeaningLeft"),
	Eps_BracedJumpingRight UMETA(DisplayName = "BracedJumpingRight"),
	Eps_BracedJumpingLeft UMETA(DisplayName = "BracedJumpingLeft"),
	Eps_BracedClimbingOver UMETA(DisplayName = "BracedClimbingOver"),
	Eps_Max UMETA(DisplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class EBowStatus : uint8
{
	Ebs_NA UMETA(DisplayName = "NA"),
	Ebs_AimingReady UMETA(DisplayName = "AimingReady"),
	Ebs_PoweringShot UMETA(DisplayName = "PoweringShot"),
	Ebs_FiringShot UMETA(DisplayName = "FiringShot"),
	Ebs_Max UMETA(DisplayName = "DefaultMax")
};
UENUM(BlueprintType)
enum class ERequestStance : uint8
{
	Ers_StandIdling UMETA(DisplayName = "StandIdling"),
	Ers_StandWalking UMETA(DisplayName = "StandWalking"),
	Ers_StandJogging UMETA(DisplayName = "StandJogging"),
	Ers_StandSprinting UMETA(DisplayName = "StandSprinting"),
	Ers_StandJumping UMETA(DisplayName = "StandJumping"),
	Ers_CrouchIdling UMETA(DisplayName = "CrouchIdling"),
	Ers_CrouchWalking UMETA(DisplayName = "CrouchWalking"),
	Ers_CrouchSprinting UMETA(DisplayName = "CrouchSprinting"),
	Ers_Aiming UMETA(DisplayName = "Aiming "),
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

	#pragma  region Components
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (DisplayName = "Spring Arm Component"))
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Camera", meta = (DisplayName = "Camera Component"))
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Parkour", meta = (DisplayName = "Arrow Right Component"))
		UArrowComponent* ArrowMoveRightComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Parkour", meta = (DisplayName = "Arrow Left Component"))
		UArrowComponent* ArrowMoveLeftComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Parkour", meta = (DisplayName = "Arrow Left Component"))
		UArrowComponent* ArrowJumpLeftComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Parkour", meta = (DisplayName = "Arrow Left Component"))
		UArrowComponent* ArrowJumpRightComp;
	
	// Player Controller
	AATHAscilaPC* AscilaPC; 
	#pragma endregion
	
	#pragma region States
	//Parent Stances
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
	EParentStance ParentStance;
	UFUNCTION(BlueprintCallable,Category = "States")
	void SetParentStanceStatus(EParentStance Status);
	EParentStance GetParentStanceStatus();
	
	// Stance Status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
	EStanceStatus StanceStatus;
	UFUNCTION(BlueprintCallable, Category = "States")
	void SetStanceStatus(EStanceStatus Status);
	EStanceStatus GetStanceStatus();

	// Parkour Status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
		EParkourStatus ParkourStatus;
	UFUNCTION(BlueprintCallable, Category = "States")
		void SetParkourStatus(EParkourStatus Status);
	EParkourStatus GetParkourStatus();
	
	// Bow Status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
		EBowStatus BowStatus;
	void SetBowStatus(EBowStatus Status);
	EBowStatus GetBowStatus();
	
	// Request Stance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
	ERequestStance RequestStance;
	void SetRequestedStatus(ERequestStance RequestedStance);
	ERequestStance GetRequestedStance();
#pragma endregion

	#pragma region Weapons
	// Weapons
	void SetYaw(float NewYaw);
	void SetPitch(float NewPitch);
	float GetMaxPitch();
	float GetMinPitch();
	bool GetIsAiming();
	#pragma endregion
	
protected:
	// Called when the game starts or	when spawned
	virtual void BeginPlay() override;

	#pragma region Mesh & Capsule Variables
	// Initialisation Variables
		// Mesh Component
	FVector TargetMeshLocation = FVector(0, 0, 0);
			// Stand
	FVector StandMeshInitialiseLocation = FVector(-9, -4.5, -81.0);
	FRotator StandMeshInitialiseRotation = FRotator(0, -90, 0);
			// Crouch
	FVector CrouchMeshInitialiseLocation = FVector(-18.0, -4.5, -64);
	FVector CrouchSprintMeshInitialiseLocation = FVector(-20, 0, -66);
			// Capsule Component
	float StandCapsuleRadius = 24.0f;
	float StandCapsuleHalfHeight = 90;
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
	float CapsuleMeshAlpha = 0.005f;
	UPROPERTY(EditAnywhere, Category = "Components|Capsule Properties")
	float CapsuleTolerance = 0.5f;
	void CapsuleMeshPropertiesChange();

	FTimerHandle CapsuleMeshProprtiesChangeTimer;
	#pragma endregion 

	// Input
	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	float InputForward = 0.0f;
	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	float InputRight = 0.0f;
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void LookRight(float value);

	#pragma region Movement Functions and Variables
	// Movement
		// Rotation
	//FRotator DefaultRotationRate = FRotator(0,1000,0);
	FRotator SprintingRotationRate = FRotator(0, 310, 0);
	void ChangeRotationRate();
		// Character Movement Speed
	void SetCharacterSpeed(float Speed);
	UPROPERTY(EditInstanceOnly, Category = "Movement Properties")
	float WalkSpeed = 130.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float JogSpeed = 520.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float SprintSpeed = 950.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float CrouchWalkSpeed = 175.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float CrouchSprintSpeed = 600.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement Properties")
	float RollSpeed = 675.0f;
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
	float DistanceToObjectAbove();
		// Jump
	void RequestJump();
	void AscilaJump();
	void JumpLaunchAscila();
	FVector LaunchForce = FVector(0, 0, 0);
	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	bool JumpWindowOpen = false;
	void DelayedSetJumpWindowF();
	FTimerHandle JumpWindowHandle;
	bool bShouldInAirJogJump = true;
	bool bSprintJumped = false;
	// Landed
	bool bNeedsToLand = false;
	FTimerHandle LandedHandle;

	#pragma endregion
	
	#pragma region Weapons

	UPROPERTY(VisibleInstanceOnly, Category = "Weapon Properties")
	// Aiming
	bool bIsAiming = false;
	float Pitch;
	UPROPERTY(EditInstanceOnly, Category = "Weapon Properties")
	float MaxPitch = 50.0f;
	UPROPERTY(EditInstanceOnly, Category = "Weapon Properties")
	float MinPitch = -70.0f;
	float Yaw;
	UPROPERTY(EditInstanceOnly, Category = "Weapon Properties")
	bool bLerpYaw = false;
	UFUNCTION(BlueprintCallable, Category = "Animation|Internal")
	void SetControllerRotationYawInput(bool SetYawTo);
	void RequestAim();
	void RequestUnAim();
	void AimIn();
	void AimOut();
	void SetAimReadyValue();
	UPROPERTY(EditInstanceOnly, Category = "Weapon Properties")
	float AimReadyAlpha = 0.001f;
	float CurrentAimReady = 0.0f;
	FTimerHandle AimingReadyHandle; // When animation blending is finished then you are aim 'ready'
	UPROPERTY(EditInstanceOnly, Category = "Weapon Properties")
	float ChangeCameraPropertiesAlpha = 0.01f;
	UPROPERTY(EditInstanceOnly, Category = "Weapon Properties")
	float SmoothCameraAlpha = 0.03f;
	FTimerHandle AimInHandle; // Camera Purposes
		
	// Firing
	void RequestDrawChange();
	void DrawBow();
	void UnDrawBow();
	void RequestFire();
	void Fire();
	bool bIsArrowDrawn = false;
	void DrawArrow();
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties|Montages")
	UAnimMontage* UnEquipBowMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties|Montages")
	UAnimMontage* DrawArrowMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties|Montages")
	UAnimMontage* FireArrowMontage;

	// Bow
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties|Bow")
		TSubclassOf<AATHBow> BowClass;

	void SpawnBow();
	FName BowSocketName = "Bow_Socket";
	#pragma endregion
	
	#pragma region Camera
	UFUNCTION(BlueprintCallable)
	void ChangeCameraProperties(float DistanceFromCamera, float CameraFOV, FVector CameraLocation, float TransitionSpeed);
	void SmoothCameraTransition();
	float CameraPropertiesTolerance = 0.5f;
	
		// Default
	float SpringCompDefaultArmLength = 400.0f;
	float CameraDefaultFOV = 90.0f;
	FVector SpringCompSocketDefaultOffset = FVector(0, 55.0, 80.0);

		// Aiming
	float SpringCompAimArmLength = 400.0f;
	float CameraAimFOV = 90.0f;
	FVector SpringCompSocketAimOffset = FVector(260.0f, 100.0f, 80.0f);
 
		// Target
	float TargetSpringCompArmLength = 400.0f;
	float TargetCameraFOV = 90.0f;
	FVector TargetSpringCompSocketOffset = FVector(0, 55.0, 80.0);

	#pragma endregion
	
	// Animation
	float PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName);
	void StopAnimMontagePlaying(UAnimMontage* AnimMontage);
	bool bIsRMRotating = false;

	#pragma region Parkour
	// States
	FName PelvisSocketName = "Pelvis_Socket";
	
		//Braced
	//bool bCanGrab = false;
	bool bIsBraced = false;
	bool bCanBracedMoveRight = false;
	bool bCanBracedMoveLeft = false;
	bool bCanBracedJumpRight = false;
	bool bCanBracedJumpLeft = false;

	// Capsule Sizes/Heights
	float BracedCapsuleRadius = 35.0f;
	float BracedCapsuleHalfHeight = 50;
	FVector BracedInitialiseMeshLocation = FVector(-9.0f,-4.5f,-141.0f);
	//Traces
		// Forward
	UFUNCTION(BlueprintCallable)
		void LedgeTraceForward();
	FVector WallTraceLocation;
	FVector WallNormal;
		// Height
	UFUNCTION(BlueprintCallable)
		void LedgeTraceHeight();
	UPROPERTY(VisibleInstanceOnly, Category = "Parkour")
	FVector WallHeightLocation;
		// Move Left Right
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void LeftMoveLedgeTracer();
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void RightMoveLedgeTracer();
		// Jump Left Right
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void LeftJumpLedgeTracer();
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void RightJumpLedgeTracer();
	// Parkour Movement
		// Braced
	void GrabLedge();
	void ExitBrace();
	void BracedClimbLedge();
	void BracedMove(float inputValue);
	void BracedJump(float inputValue);
	
	#pragma endregion

	#pragma region Move To
	void CallSetMoveToParameters(FVector TargetLocation, FRotator TargetRotation, float Speed);
	FVector TargetMoveToLocation = FVector(0, 0, 0);
	FRotator TargetMoveToRotation = FRotator(0, 0, 0);
	
	FTimerHandle MoveToHandle;
	
	void MoveTo();
	#pragma endregion
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Override FaceRotation For Yaw Lerp
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;
	
	// Landed
	void SetNeedsToLandT();
	void SetNeedsToLandF();
	bool GetNeedsToLand();
	virtual void Landed(const FHitResult& Hit) override;
	UPROPERTY(VisibleInstanceOnly, Category = "Animation")
	bool bShouldRollLand = false;
	void SetShouldRollLand(bool ShouldRollLand);
	bool bShouldHardLand = false;
	void SetShouldHardLand(bool ShouldHardLand);
	bool bShouldSprintRollLand = false;

	// Weapons
	void ReDrawArrow();
	void SetArrowDrawnVariable(bool isArrowDrawn);
	float GetPitch();
	APlayerCameraManager* GetCameraManager();
	AATHBow* EquippedBow;
	
	//Jump
	void SetJumpWindowT();
	void SetJumpWindowF(bool ShoulDelay);
	bool GetShouldInAirJogJump();
	bool GetSprintJumped();

	// Animation
	UFUNCTION(BlueprintCallable, Category= "Animation|Internal")
	void SetPCRootMotionRotation(bool NeedsRootMotionRotation);

	// Parkour
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void EnterRMState();
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void ExitRMState();
	//bool GetCanGrab();
	UFUNCTION(BlueprintCallable, Category = "Parkour")
	void CapsuleMeshPropertiesTimer();
	UPROPERTY(EditInstanceOnly, Category = "Parkour")
	float ZHeightParkourTest = 88.0f;

};
