// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/ATHAscila.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"
#include "Animation/AnimInstance.h"
#include "Character/Ascila/ATHAscilaPC.h"
#include "AscilaTheHuntress/AscilaTheHuntress.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/BowAndArrow/ATHBow.h"


// Sets default values
AATHAscila::AATHAscila()
{	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetWorldLocationAndRotation(StandMeshInitialiseLocation, StandMeshInitialiseRotation);

	//Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmComp->SocketOffset
	//SpringArmComp->SetWorldLocation(SpringArmInitialiseLocation);
	SpringArmComp->SocketOffset = SpringCompSocketDefaultOffset;
	SpringArmComp->TargetArmLength = SpringCompDefaultArmLength;

	//Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//CameraComp->bUsePawnControlRotation = false;

	//Capsule Component
	GetCapsuleComponent()->SetCapsuleRadius(StandCapsuleRadius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(StandCapsuleHalfHeight);

	// Movement Component
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AATHAscila::BeginPlay()
{
	Super::BeginPlay();
	SpawnBow();
}

// Called every frame
void AATHAscila::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AATHAscila::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Axis Events
	PlayerInputComponent->BindAxis("MoveForward", this, &AATHAscila::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AATHAscila::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AATHAscila::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AATHAscila::LookRight);
	//Action Mappings
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AATHAscila::RequestSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AATHAscila::SprintReleased);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AATHAscila::RequestCrouchChange);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AATHAscila::RequestAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AATHAscila::RequestUnAim);
	PlayerInputComponent->BindAction("DrawArrow", IE_Pressed, this, &AATHAscila::RequestDrawChange);
	PlayerInputComponent->BindAction("DrawArrow", IE_Released, this, &AATHAscila::RequestFire);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AATHAscila::RequestJump);
	
	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AATHAscila::RequestDrawBow);
}

	#pragma region  General Input

void AATHAscila::MoveForward(float value)
{
	if(ParentStance != EParentStance::Eps_Parkouring)
	{
		// Pressing forward/backward...
		if (value != 0)
		{
			if (ParentStance == EParentStance::Eps_Parkouring)
			{

			}
			else
			{
			}
			bIsMovingForwardBackward = true;

			if (value > 0) { bIsMovingforward = true; }
			else if (value < 0) { bIsMovingforward = false; }

			if (ParentStance == EParentStance::Eps_Standing)
			{
				if (StanceStatus == EStanceStatus::Ess_StandSprinting && value > 0)
				{

				}
				else if (StanceStatus == EStanceStatus::Ess_StandIdleJumping ||
					StanceStatus == EStanceStatus::Ess_StandJogJumping ||
					StanceStatus == EStanceStatus::Ess_StandSprintJumping)
				{

				}
				else
				{
					SetStanceStatus(EStanceStatus::Ess_StandJogging);
				}
			}
			else if (ParentStance == EParentStance::Eps_Crouching)
			{
				if (StanceStatus == EStanceStatus::Ess_CrouchSprinting && value > 0)
				{

				}
				else
				{
					SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
				}
			}

			bIdleCheck = true;
		}
		// Not pressing forward/backward
		else
		{
			if (StanceStatus != EStanceStatus::Ess_LandRolling)
			{

				bIsMovingForwardBackward = false;

				// not moving forward and right
				if (!bIsMovingForwardBackward && !bIsMovingRightLeft)
				{
					// Is idle check animation
					IdleCheck();
				}
				else
				{

				}
			}
			else
			{
				AddMovementInput(GetActorForwardVector() * 1);
			}
		}

		AddMovementInput(GetActorForwardVector()*value);
	}
}
void AATHAscila::MoveRight(float value)
{
	if (ParentStance != EParentStance::Eps_Parkouring)
	{
		if (value != 0)
		{
			bIsMovingRightLeft = true;

			if (value > 0) { bIsMovingRight = true; }
			else if (value < 0) { bIsMovingRight = false; }

			if (ParentStance == EParentStance::Eps_Standing)
			{
				if (StanceStatus == EStanceStatus::Ess_StandIdleJumping ||
					StanceStatus == EStanceStatus::Ess_StandJogJumping ||
					StanceStatus == EStanceStatus::Ess_StandSprintJumping)
				{

				}
				else
				{
					SetStanceStatus(EStanceStatus::Ess_StandJogging);
				}

			}
			else if (ParentStance == EParentStance::Eps_Crouching)
			{
				SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
			}

			bIdleCheck = true;
		}
		else
		{
			bIsMovingRightLeft = false;
		}

		AddMovementInput(GetActorRightVector()* value);
	}
}
void AATHAscila::LookUp(float value)
{
	if (Pitch != MaxPitch && Pitch != MinPitch)
	{
		AddControllerPitchInput(value);
	}
	else if (Pitch != MaxPitch)
	{
		if (value < 0)
		{
			AddControllerPitchInput(value);
		}
	}
	else if (Pitch != MinPitch)
	{
		if (value > 0)
		{
			AddControllerPitchInput(value);
		}
	}
}
void AATHAscila::LookRight(float value)
{
	AddControllerYawInput(value);
}
void AATHAscila::RequestSprint()
{
	if (ParentStance != EParentStance::Eps_Dead)
	{
		if(ParentStance != EParentStance::Eps_InAir)
		{
			if (StanceStatus != EStanceStatus::Ess_StandSprinting || StanceStatus != EStanceStatus::Ess_CrouchSprinting)
			{
				if (StanceStatus != EStanceStatus::Ess_StandIdling || StanceStatus != EStanceStatus::Ess_CrouchIdling)
				{
					if (ParentStance != EParentStance::Eps_Rolling)
					{
						if (bIsMovingForwardBackward && !bIsMovingRightLeft)
						{
							Sprint();
						}
					}
					else
					{
						if (ParentStance == EParentStance::Eps_Standing)
						{
							SetRequestedStatus(ERequestStance::Ers_StandSprinting);
						}
						else
						{
							SetRequestedStatus(ERequestStance::Ers_CrouchSprinting);
						}

					}
				}
			}
		}
	}
}
void AATHAscila::Sprint()
{
	if(ParentStance == EParentStance::Eps_Standing)
	{
		SetStanceStatus(EStanceStatus::Ess_StandSprinting);
		SetRequestedStatus(ERequestStance::Ers_NA);
	}
	else if(ParentStance == EParentStance::Eps_Crouching)
	{
		SetStanceStatus(EStanceStatus::Ess_CrouchSprinting);
		GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
		SetRequestedStatus(ERequestStance::Ers_NA);
	}
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}
void AATHAscila::SprintReleased()
{
	if(StanceStatus == EStanceStatus::Ess_StandSprinting)
	{
		if (bIsMovingForwardBackward)
		{
			SetStanceStatus(EStanceStatus::Ess_StandJogging);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_StandIdling);
		}

		PlayAnimMontage(UnEquipBowMontage, 1.0f, NAME_None);

	}
	else if(StanceStatus == EStanceStatus::Ess_CrouchSprinting)
	{
		if (bIsMovingForwardBackward)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchIdling);
		}
		
		GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
	}
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}
void AATHAscila::RequestCrouchChange()
{
	// If crouching
	if(ParentStance == EParentStance::Eps_Crouching)
	{
		if (ParentStance != EParentStance::Eps_Dead)
		{
			if (ParentStance != EParentStance::Eps_InAir)
			{
				if (ParentStance != EParentStance::Eps_Rolling)
				{
					if (DistanceToObjectAbove() > 169.0f)
					{
						AscilaUnCrouch();
					}
				}
				else
				{
					if (bIsMovingRightLeft || bIsMovingForwardBackward)
					{
						SetRequestedStatus(ERequestStance::Ers_CrouchWalking);
					}
					else
					{
						SetRequestedStatus(ERequestStance::Ers_CrouchIdling);
					}
				}
			}
		}
	}
	// If not crouching
	else
	{
		if (ParentStance != EParentStance::Eps_Dead)
		{
			if (ParentStance != EParentStance::Eps_Rolling)
			{
				if (ParentStance != EParentStance::Eps_Parkouring)
				{
					AscilaCrouch();
				}
				else
				{
					ExitBrace();
				}
			}
			else
			{
				if (bIsMovingRightLeft || bIsMovingForwardBackward)
				{
					SetRequestedStatus(ERequestStance::Ers_CrouchWalking);
				}
				else
				{
					SetRequestedStatus(ERequestStance::Ers_CrouchIdling);
				}

			}
		}
	}
	
}
void AATHAscila::AscilaCrouch()
{
	if(ParentStance == EParentStance::Eps_Standing)
	{

		if (StanceStatus == EStanceStatus::Ess_StandIdling)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchIdling);
			GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
		}
		else if (StanceStatus == EStanceStatus::Ess_StandJogging)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
			GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchSprinting);
			GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
		}
		
		SetParentStanceStatus(EParentStance::Eps_Crouching);
	}
	
	SetRequestedStatus(ERequestStance::Ers_NA);
}
void AATHAscila::AscilaUnCrouch()
{
	if(ParentStance == EParentStance::Eps_Crouching)
	{
		if (StanceStatus == EStanceStatus::Ess_CrouchIdling)
		{
			SetStanceStatus(EStanceStatus::Ess_StandIdling);
			
		}
		else if(StanceStatus == EStanceStatus::Ess_CrouchWalking)
		{
			SetStanceStatus(EStanceStatus::Ess_StandJogging);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_StandSprinting);
		}
		
		SetParentStanceStatus(EParentStance::Eps_Standing);
		GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
	}
	
	SetRequestedStatus(ERequestStance::Ers_NA);
}
void AATHAscila::RequestJump()
{
	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (ParentStance == EParentStance::Eps_Parkouring && bIsMovingforward)
		{
			BracedClimbLedge();
			//UE_LOG(LogTemp, Error, TEXT("JUMPED"));
		}		
		else if (ParentStance != EParentStance::Eps_InAir)
		{
			if(StanceStatus != EStanceStatus::Ess_StandIdleJumping && StanceStatus != EStanceStatus::Ess_StandJogJumping &&
				StanceStatus != EStanceStatus::Ess_StandSprintJumping)
			{
				if (ParentStance != EParentStance::Eps_Rolling)
				{
					AscilaJump();
				}
				else
				{
					//SetRequestedStatus(ERequestStance::Ers_StandJumping);
				}
			}
			
		}
		else if(ParentStance == EParentStance::Eps_InAir && JumpWindowOpen)
		{
			AscilaJump();
		}
	}
}
void AATHAscila::AscilaJump()
{
	if(ParentStance == EParentStance::Eps_Standing || ParentStance == EParentStance::Eps_Crouching)
	{
		// Idling Jump
		if (StanceStatus == EStanceStatus::Ess_StandIdling || StanceStatus == EStanceStatus::Ess_CrouchIdling)
		{
			LaunchForce.X = 0;
			LaunchForce.Y = 0;
			LaunchForce.Z = 700.0f;
			SetParentStanceStatus(EParentStance::Eps_Standing);
			SetStanceStatus(EStanceStatus::Ess_StandIdleJumping);
			GetWorldTimerManager().SetTimer(JumpWindowHandle, this, &AATHAscila::JumpLaunchAscila, 0.35f, false);
		}
		// Jogging Jump
		else if (StanceStatus == EStanceStatus::Ess_StandJogging || StanceStatus == EStanceStatus::Ess_CrouchWalking
			|| StanceStatus == EStanceStatus::Ess_CrouchSprinting)
		{
			LaunchForce.X = GetActorForwardVector().X * 500.0f;
			LaunchForce.Y = GetActorForwardVector().Y * 500.0f;
			LaunchForce.Z = 800.0f;
			SetParentStanceStatus(EParentStance::Eps_Standing);
			SetStanceStatus(EStanceStatus::Ess_StandJogJumping);
			JumpLaunchAscila();
		}
		// Sprinting jump
		else
		{
			LaunchForce.X = GetActorForwardVector().X * 1200.0f;
			LaunchForce.Y = GetActorForwardVector().Y * 1200.0f;
			LaunchForce.Z = 900.0f;
			SetParentStanceStatus(EParentStance::Eps_Standing);
			SetStanceStatus(EStanceStatus::Ess_StandSprintJumping);
			bShouldSprintRollLand = true;
			bSprintJumped = true;
			JumpLaunchAscila();
		}
	}
	else
	{
		if(bShouldInAirJogJump)
		{

			LaunchForce.X = GetActorForwardVector().X * 500.0f;
			LaunchForce.Y = GetActorForwardVector().Y * 500.0f;
			LaunchForce.Z = 800.0f;
			SetStanceStatus(EStanceStatus::Ess_StandJogJumping);
			JumpLaunchAscila();
		}
		else
		{
			LaunchForce.X = GetActorForwardVector().X * 1600.0f;
			LaunchForce.Y = GetActorForwardVector().Y * 1600.0f;
			LaunchForce.Z = 900.0f;
			SetStanceStatus(EStanceStatus::Ess_StandSprintJumping);
			bShouldSprintRollLand = true;
			JumpLaunchAscila();
		}
	}
	
}
void AATHAscila::JumpLaunchAscila()
{
	LaunchCharacter(LaunchForce, true, true);	
}
void AATHAscila::SetJumpWindowT()
{
	JumpWindowOpen = true;
}
void AATHAscila::SetJumpWindowF(bool ShouldDelay)
{
	if(ShouldDelay)
	{
		GetWorldTimerManager().SetTimer(JumpWindowHandle, this, &AATHAscila::DelayedSetJumpWindowF, 0.2f, false);
	}
	else
	{
		JumpWindowOpen = false;
	}
}
void AATHAscila::DelayedSetJumpWindowF()
{
	JumpWindowOpen = false;
}

	#pragma endregion 

	#pragma  region States

// Parent Stance
void AATHAscila::SetParentStanceStatus(EParentStance Status)
{
	ParentStance = Status;
}
EParentStance AATHAscila::GetParentStanceStatus()
{
	return ParentStance;
}
// Stance Status
void AATHAscila::SetStanceStatus(EStanceStatus Status)
{
	StanceStatus = Status;

	switch (StanceStatus)
	{
	case EStanceStatus::Ess_StandIdling:
		if(bIsAiming)
		{
			SetCharacterSpeed(WalkSpeed);
		}
		else
		{
			SetCharacterSpeed(JogSpeed);
		}
		bShouldInAirJogJump = true;
		break;
	case EStanceStatus::Ess_StandJogging:
		if (bIsAiming)
		{
			SetCharacterSpeed(WalkSpeed);
		}
		else
		{
			SetCharacterSpeed(JogSpeed);
		}
		bShouldInAirJogJump = true;
		break;
	case EStanceStatus::Ess_StandSprinting:
		SetCharacterSpeed(SprintSpeed);
		PlayAnimMontage(UnEquipBowMontage, -1.0f, NAME_None);
		bShouldInAirJogJump = false;
		break;
	case EStanceStatus::Ess_CrouchIdling:
		SetCharacterSpeed(CrouchWalkSpeed);
		break;
	case EStanceStatus::Ess_CrouchWalking:
		SetCharacterSpeed(CrouchWalkSpeed);
		break;
	case EStanceStatus::Ess_CrouchSprinting:
		SetCharacterSpeed(CrouchSprintSpeed);
		break;
	case EStanceStatus::Ess_LandRolling:
		SetCharacterSpeed(RollSpeed);
		break;
	}
}
EStanceStatus AATHAscila::GetStanceStatus()
{
	return StanceStatus;
}
// Parkour Status
void AATHAscila::SetParkourStatus(EParkourStatus Status)
{
	ParkourStatus = Status;
}
EParkourStatus AATHAscila::GetParkourStatus()
{
	return ParkourStatus;
}
// Bow Status
void AATHAscila::SetBowStatus(EBowStatus Status)
{
	BowStatus = Status;
}
EBowStatus AATHAscila::GetBowStatus()
{
	return BowStatus;
}
// Requested Stance
void AATHAscila::SetRequestedStatus(ERequestStance RequestedStance)
{
	RequestStance = RequestedStance;
}
ERequestStance AATHAscila::GetRequestedStance()
{
	return RequestStance;
}

	#pragma endregion 

	#pragma region Movement

// Landed
void AATHAscila::SetNeedsToLandT()
{
	bNeedsToLand = true;
}
void AATHAscila::SetNeedsToLandF()
{
	bNeedsToLand = false;
}
bool AATHAscila::GetNeedsToLand()
{
	return bNeedsToLand;
}
void AATHAscila::Landed(const FHitResult & Hit)
{
	if(bShouldSprintRollLand)
	{
		SetParentStanceStatus(EParentStance::Eps_Rolling);
		SetStanceStatus(EStanceStatus::Ess_LandRolling);
		bShouldSprintRollLand = false;
	}
	else
	{
		if (bShouldRollLand)
		{
			SetParentStanceStatus(EParentStance::Eps_Rolling);
			SetStanceStatus(EStanceStatus::Ess_LandRolling);
		}
		else if (bShouldHardLand)
		{
			SetParentStanceStatus(EParentStance::Eps_Rolling);
			SetStanceStatus(EStanceStatus::Ess_LandHard);
		}
		else
		{
			SetParentStanceStatus(EParentStance::Eps_Standing);
			SetStanceStatus(EStanceStatus::Ess_StandIdling);
		}
	}
	bSprintJumped = false;
	GetWorldTimerManager().SetTimer(LandedHandle, this, &AATHAscila::SetNeedsToLandF, 0.25f, false);
}
void AATHAscila::SetShouldRollLand(bool ShouldRollLand)
{
	bShouldRollLand = ShouldRollLand;
}
void AATHAscila::SetShouldHardLand(bool ShouldHardLand)
{
	bShouldHardLand = ShouldHardLand;
}

void AATHAscila::SetCharacterSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}
void AATHAscila::IdleCheck()
{
	if (bIdleCheck)
	{
		switch (ParentStance)
		{
		case EParentStance::Eps_Standing:
			SetStanceStatus(EStanceStatus::Ess_StandIdling);
			break;
		case EParentStance::Eps_Crouching:
			SetStanceStatus(EStanceStatus::Ess_CrouchIdling);
			break;
		}

		bIdleCheck = false;
	}
}
bool AATHAscila::GetShouldInAirJogJump()
{
	return bShouldInAirJogJump;
}
bool AATHAscila::GetSprintJumped()
{
	return bSprintJumped;
}
void AATHAscila::SetPCRootMotionRotation(bool NeedsRootMotionRotation)
{
	AscilaPC = Cast<AATHAscilaPC>(GetWorld()->GetFirstPlayerController());
	if (AscilaPC != nullptr)
	{
		AscilaPC->SetIsRootMotionRotating(NeedsRootMotionRotation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No PC found"));
	}
}

	#pragma endregion 

	#pragma region Weapons and Aiming

void AATHAscila::SetYaw(float NewYaw)
{
	Yaw = NewYaw;
}
void AATHAscila::SetPitch(float NewPitch)
{
	Pitch = NewPitch;
}
float AATHAscila::GetPitch()
{
	return Pitch;
}
float AATHAscila::GetMaxPitch()
{
	return MaxPitch;
}
float AATHAscila::GetMinPitch()
{
	return MinPitch;
}
bool AATHAscila::GetIsAiming()
{
	return bIsAiming;
}
void AATHAscila::RequestAim()
{
	// also add jumping restriction and swimming
	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (ParentStance != EParentStance::Eps_Rolling)
		{
			if(ParentStance == EParentStance::Eps_Standing)
			{
				if(StanceStatus == EStanceStatus::Ess_StandSprinting)
				{
					SetStanceStatus(EStanceStatus::Ess_StandWalking);
				}
			}
			else if (ParentStance == EParentStance::Eps_Crouching)
			{
				if (StanceStatus == EStanceStatus::Ess_CrouchSprinting)
				{
					SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
				}
			}
			
			AimIn();
		}
		else
		{
			//SetRequestedStatus(ERequeststance)
		}
	}
}
void AATHAscila::RequestUnAim()
{
	AimOut();
}
void AATHAscila::AimIn()
{
	bIsAiming = true;
	SetStanceStatus(GetStanceStatus());
	
	if (!bIsArrowDrawn)
	{
		DrawArrow();
	}
	else
	{
		AimReadyAlpha = 0.001f;
	}
	
	ChangeCameraProperties(SpringCompAimArmLength,CameraAimFOV,SpringCompSocketAimOffset, ChangeCameraPropertiesAlpha);

	GetWorldTimerManager().SetTimer(AimingReadyHandle, this, &AATHAscila::SetAimReadyValue, AimReadyAlpha, true);
}
void AATHAscila::AimOut()
{
	bIsAiming = false;
	ChangeCameraProperties(SpringCompDefaultArmLength, CameraDefaultFOV, SpringCompSocketDefaultOffset, ChangeCameraPropertiesAlpha);
	
	SetBowStatus(EBowStatus::Ebs_NA);
	GetWorldTimerManager().SetTimer(AimingReadyHandle, this, &AATHAscila::SetAimReadyValue, AimReadyAlpha, true);
	StopAnimMontagePlaying(DrawArrowMontage);
	
	SetStanceStatus(GetStanceStatus());

	
}
void AATHAscila::SetAimReadyValue()
{
	
	if(bIsAiming)
	{
		if(!(FMath::IsNearlyEqual(CurrentAimReady, 100.0f,0.5f)))
		{
			CurrentAimReady = FMath::Lerp(CurrentAimReady, 100.0f, 0.02);
		}
		else
		{
			SetBowStatus(EBowStatus::Ebs_AimingReady);
			bIsArrowDrawn = true;
			GetWorldTimerManager().ClearTimer(AimingReadyHandle);
		}
	}
	else
	{
		if (!(FMath::IsNearlyEqual(CurrentAimReady, 0.0f, 0.5f)))
		{
			CurrentAimReady = FMath::Lerp(CurrentAimReady, 0.0f, 0.02);
		}
		else
		{
			GetWorldTimerManager().ClearTimer(AimingReadyHandle);
		}
	}
}
void AATHAscila::RequestDrawChange()
{
	// Shooting arrows makes you lose Health ? as a way to balance out otherwise you just continously shoot arrows ezpz?
	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (ParentStance != EParentStance::Eps_Rolling)
		{
			if(bIsAiming)
			{
				if(BowStatus == EBowStatus::Ebs_AimingReady)
				{
					DrawBow();
				}
			}
		}
		else
		{
			
		}
	}
}
void AATHAscila::DrawBow()
{
	SetBowStatus(EBowStatus::Ebs_PoweringShot);
}
void AATHAscila::UnDrawBow()
{
	SetBowStatus(EBowStatus::Ebs_NA);
}
void AATHAscila::DrawArrow()
{
	AimReadyAlpha = 0.003f;
	PlayAnimMontage(DrawArrowMontage, 1.0f, NAME_None);
	GetWorldTimerManager().SetTimer(AimingReadyHandle, this, &AATHAscila::SetAimReadyValue, AimReadyAlpha, true);
}
void AATHAscila::ReDrawArrow()
{
	AimReadyAlpha = 0.003f;
	PlayAnimMontage(DrawArrowMontage, 1.0f, NAME_None);
	CurrentAimReady = 0;
	GetWorldTimerManager().SetTimer(AimingReadyHandle, this, &AATHAscila::SetAimReadyValue, AimReadyAlpha, true);
}
void AATHAscila::SetArrowDrawnVariable(bool isArrowDrawn)
{
	bIsArrowDrawn = isArrowDrawn;
}
void AATHAscila::RequestFire()
{
	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (ParentStance != EParentStance::Eps_Rolling)
		{
			if (bIsAiming)
			{
				if (BowStatus == EBowStatus::Ebs_PoweringShot)
				{
					Fire();
				}
			}
		}
		else
		{

		}
	}
}
void AATHAscila::Fire()
{
	SetBowStatus(EBowStatus::Ebs_FiringShot);
	PlayAnimMontage(FireArrowMontage, 1.0f, NAME_None);
	/*EquippedBow = Cast<AATHBow>(EquippedBow);
	if(EquippedBow != nullptr)
	{
		EquippedBow->FireArrow();
		UE_LOG(LogTemp, Error, TEXT("Successful Cast"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed Cast"));
	}*/
}
void AATHAscila::SpawnBow()
{

	UWorld* world = GetWorld();
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	FRotator rotator;
	FVector spawnLocation = this->GetActorLocation();

	EquippedBow = world->SpawnActor<AATHBow>(BowClass, spawnLocation, rotator, spawnParams);
	EquippedBow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, BowSocketName);
}

	#pragma endregion 

	#pragma  region Utilities / Animation

void AATHAscila::ChangeCameraProperties(float DistanceFromCamera, float CameraFOV, FVector CameraLocation, float TransitionSpeed)
{
	TargetSpringCompSocketOffset = CameraLocation;
	TargetSpringCompArmLength = DistanceFromCamera;
	TargetCameraFOV = CameraFOV;

	GetWorldTimerManager().SetTimer(AimInHandle, this, &AATHAscila::SmoothCameraTransition, TransitionSpeed, true);


}
void AATHAscila::SmoothCameraTransition()
{
	bool IsEqualArmLength = false;
	bool IsEqualCameraFOV = false;
	bool IsEqualSocketOffset = false;
	
	if (!(FMath::IsNearlyEqual(SpringArmComp->TargetArmLength, TargetSpringCompArmLength, CameraPropertiesTolerance)))
	{
		SpringArmComp->TargetArmLength = FMath::Lerp(SpringArmComp->TargetArmLength, TargetSpringCompArmLength, SmoothCameraAlpha);
	}
	else
	{
		IsEqualArmLength = true;
	}
	if (!(FMath::IsNearlyEqual(CameraComp->FieldOfView, TargetCameraFOV, CameraPropertiesTolerance)))
	{
		CameraComp->FieldOfView = FMath::Lerp(CameraComp->FieldOfView, TargetCameraFOV, SmoothCameraAlpha);
	}
	else
	{
		IsEqualCameraFOV = true;
	}
	if (FVector::Dist(SpringArmComp->SocketOffset, TargetSpringCompSocketOffset) > CameraPropertiesTolerance)
	{
		SpringArmComp->SocketOffset = FMath::Lerp(SpringArmComp->SocketOffset, TargetSpringCompSocketOffset, SmoothCameraAlpha);
	}
	else
	{
		IsEqualSocketOffset = true;
	}
	
	if (IsEqualSocketOffset && IsEqualCameraFOV  && IsEqualArmLength)
	{
		GetWorldTimerManager().ClearTimer(AimInHandle);

	}
}
APlayerCameraManager* AATHAscila::GetCameraManager()
{
	APlayerCameraManager *CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	return CamManager;
}
void AATHAscila::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	Super::FaceRotation(NewControlRotation, DeltaTime);
}
void AATHAscila::SetControllerRotationYawInput(bool SetYawTo)
{
	bLerpYaw = SetYawTo;
	bUseControllerRotationYaw = bLerpYaw;
}
float AATHAscila::DistanceToObjectAbove()
{
	FVector StartLocation = FVector(0, 0, 0);
	FVector EndLocation = FVector(0, 0, 0);
	FName RootName = "root";
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;

	FHitResult Hit;
	StartLocation = FVector(GetMesh()->GetBoneLocation(RootName).X, GetMesh()->GetBoneLocation(RootName).Y,
		GetMesh()->GetBoneLocation(RootName).Z);
	EndLocation.X = StartLocation.X;
	EndLocation.Y = StartLocation.Y;
	EndLocation.Z = StartLocation.Z + 200.0f;
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		return Hit.Distance;
	}
	else
	{
		return 1000;
	}
}
float AATHAscila::PlayAnimMontage(UAnimMontage * AnimMontage, float InPlayRate, FName StartSectionName)
{

	UAnimInstance* AnimInstance;

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	else
	{
		AnimInstance = nullptr;
	}

	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		// Start at a given Section if given
		if (StartSectionName != NAME_None)
		{
			AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
		}
		return Duration;
	}
	return 0.f;
}
void AATHAscila::StopAnimMontagePlaying(UAnimMontage* AnimMontage)
{

	UAnimInstance* AnimInstance;

	if (GetMesh())
	{
		AnimInstance = GetMesh()->GetAnimInstance();
	}
	else
	{
		AnimInstance = nullptr;
	}
	if (AnimMontage && AnimInstance)
	{
		if (AnimInstance->Montage_IsPlaying(AnimMontage))
		{
			AnimInstance->Montage_Stop(0.25f, AnimMontage);
		}

	}
}
void AATHAscila::CallSetMoveToParameters(FVector TargetLocation, FRotator TargetRotation, float Speed)
{
	TargetMoveToLocation = TargetLocation;
	TargetMoveToRotation = TargetRotation;

	GetWorldTimerManager().SetTimer(MoveToHandle,this,&AATHAscila::MoveTo, Speed, true);
}
void AATHAscila::MoveTo()
{
	bool IsEqualLocation = false;
	bool IsEqualRotation = false;

	FTransform TargetTransform;
	
	TargetTransform.SetRotation(TargetMoveToRotation.GetNormalized().Quaternion());;
	TargetTransform.SetLocation(TargetMoveToLocation);
	TargetTransform.SetScale3D(GetActorScale());
	
	/*if (FVector::Dist(GetActorLocation(), TargetMoveToLocation) > 0.5)
	{
		SetActorLocation(FMath::Lerp(GetActorLocation(), TargetMoveToLocation, 0.05));
	}*/

	if (!(UKismetMathLibrary::NearlyEqual_TransformTransform(GetActorTransform(), TargetTransform, 0.5, 0.5, 0.1)))
	{

		SetActorRelativeRotation(FMath::Lerp(GetActorRotation(), TargetMoveToRotation, 0.05));
		SetActorLocation(FMath::Lerp(GetActorLocation(), TargetMoveToLocation, 0.05));
	}
	else
	{
		IsEqualLocation = true;
		IsEqualRotation = true;
	}

	if(IsEqualLocation && IsEqualRotation)
	{
		UE_LOG(LogTemp, Error, TEXT("ENDED MOVE TO TIMER"));
		GetWorldTimerManager().ClearTimer(MoveToHandle);
	}
}
void AATHAscila::CapsuleMeshPropertiesChange()
{
	if (ParentStance == EParentStance::Eps_Crouching)
	{
		if (StanceStatus == EStanceStatus::Ess_CrouchSprinting)
		{
			TargetCapsuleHalfHeight = CrouchSprintCapsuleHalfHeight;
			TargetCapsuleRadius = CrouchSprintCapsuleRadius;
			TargetMeshLocation = CrouchSprintMeshInitialiseLocation;
		}
		else
		{
			TargetCapsuleHalfHeight = CrouchCapsuleHalfHeight;
			TargetCapsuleRadius = CrouchCapsuleRadius;
			TargetMeshLocation = CrouchMeshInitialiseLocation;
		}
	}
	else if (ParentStance == EParentStance::Eps_Standing)
	{
		TargetCapsuleHalfHeight = StandCapsuleHalfHeight;
		TargetCapsuleRadius = StandCapsuleRadius;
		TargetMeshLocation = StandMeshInitialiseLocation;
	}
	else if (ParentStance == EParentStance::Eps_InAir)
	{
		TargetCapsuleHalfHeight = StandCapsuleHalfHeight;
		TargetCapsuleRadius = StandCapsuleRadius;
		TargetMeshLocation = StandMeshInitialiseLocation;
	}
	else if (ParentStance == EParentStance::Eps_Rolling)
	{

	}
	else if (ParentStance == EParentStance::Eps_Parkouring)
	{
		if (ParkourStatus == EParkourStatus::Eps_BracedIdling)
		{
			TargetCapsuleHalfHeight = BracedCapsuleHalfHeight;
			TargetCapsuleRadius = BracedCapsuleRadius;
			TargetMeshLocation = BracedInitialiseMeshLocation;
		}
	}


	bool IsEqualHalfeight = false;
	bool IsEqualRadius = false;
	bool IsLocation = false;

	if (!(FMath::IsNearlyEqual(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetCapsuleHalfHeight, CapsuleTolerance)))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetCapsuleHalfHeight, HeightAlpha));
	}
	else
	{
		IsEqualHalfeight = true;
	}
	if (!(FMath::IsNearlyEqual(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), TargetCapsuleRadius, CapsuleTolerance)))
	{
		GetCapsuleComponent()->SetCapsuleRadius(FMath::Lerp(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), TargetCapsuleRadius, RadiusAlpha));
	}
	else
	{
		IsEqualRadius = true;
	}
	if (FVector::Dist(GetMesh()->GetRelativeLocation(), TargetMeshLocation) > CapsuleTolerance)
	{
		GetMesh()->SetRelativeLocation(FMath::Lerp(GetMesh()->GetRelativeLocation(), TargetMeshLocation, LocationAlpha));
	}
	else
	{
		IsLocation = true;
	}

	if (IsEqualHalfeight && IsEqualRadius && IsLocation)
	{
		GetWorldTimerManager().ClearTimer(CapsuleMeshProprtiesChangeTimer);
	}
}
void AATHAscila::CapsuleMeshPropertiesTimer()
{
	GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
}
	#pragma endregion

	#pragma region Parkour

// Traces
void AATHAscila::LedgeTraceForward()
{
	//Trace Params
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetActorRotation());
	FVector EndLocation = FVector(ForwardVector.X * 150, ForwardVector.Y * 150, ForwardVector.Z) + StartLocation;
	TArray<AActor*> MakeArray;
	FHitResult Hit;

	if(UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation,20,COLLISION_LEDGE, false, MakeArray,
		EDrawDebugTrace::ForOneFrame,Hit,true, FLinearColor::Red, FLinearColor::Green,0))
	{
		WallTraceLocation = Hit.Location;
		WallNormal = Hit.Normal;
	}
}
void AATHAscila::LedgeTraceHeight()
{
	// Tracer starts from the top and traces down!
	//Trace Params
	float TraceHeight = 500.0f;
	float ForwardDistance = 70.0f;
	FVector StartLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + TraceHeight);
	
	FVector MidLocation = (UKismetMathLibrary::GetForwardVector(GetActorRotation())* ForwardDistance) + StartLocation;
	FVector EndLocation = FVector(MidLocation.X, MidLocation.Y, MidLocation.Z - TraceHeight);
	TArray<AActor*> MakeArray;
	FHitResult Hit;

	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), MidLocation, EndLocation, 20, COLLISION_LEDGE, false, MakeArray,
		EDrawDebugTrace::ForOneFrame, Hit, true, FLinearColor::Red, FLinearColor::Green, 0))
	{
		WallHeightLocation = Hit.Location;

		// This checks to see if the character Pelvis Location.Z is a certain height below the Ledge Location.Z
		float PelvisToLedgeHeightDifferenceLimit = -100.f;
		float PelvisZLocation = UKismetMathLibrary::Abs(GetMesh()->GetSocketLocation(PelvisSocketName).Z);
		float WallHeightZLocation = UKismetMathLibrary::Abs(WallHeightLocation.Z);			
		float PelvisToLedgeHeightDifference = WallHeightZLocation - PelvisZLocation;


		bool bIsPelvisCloseEnoughToLedge = UKismetMathLibrary::InRange_FloatFloat(PelvisToLedgeHeightDifference,PelvisToLedgeHeightDifferenceLimit,
			0, true,true);
		
		UE_LOG(LogTemp, Error, TEXT("%f"), PelvisToLedgeHeightDifference);
		
		if(bIsPelvisCloseEnoughToLedge)
		{
			if(bIsClimbing)
			{

			}
			else
			{
				GrabLedge();
			}
		}
	}
	else
	{

	}
}

// Movement Events
void AATHAscila::GrabLedge()
{
	if(ParkourStatus != EParkourStatus::Eps_BracedIdling && ParkourStatus != EParkourStatus::Eps_BracedClimbingOver)
	{
		EnterRMState();
		
		bCanGrab = true;

		SetParentStanceStatus(EParentStance::Eps_Parkouring);
		SetParkourStatus(EParkourStatus::Eps_BracedIdling);

		FVector WallNormalAdjusted = WallNormal * FVector(22.0f, 22.0f, 0);
		
		FVector TargetRelativeLocation2 = FVector(WallNormalAdjusted.X + WallTraceLocation.X, WallNormalAdjusted.Y + WallTraceLocation.Y,
			WallHeightLocation.Z - ZHeightParkourTest);

		FRotator TargetRelativeRotation = FRotator(WallNormal.ToOrientationRotator().Roll, WallNormal.ToOrientationRotator().Pitch,
			WallNormal.ToOrientationRotator().Yaw);

		FLatentActionInfo info = FLatentActionInfo();
		info.CallbackTarget = this;
		
		UKismetSystemLibrary::MoveComponentTo(GetCapsuleComponent(), TargetRelativeLocation2, GetActorRotation(), false, false, 0.13f, false, EMoveComponentAction::Move, info);
		//CallSetMoveToParameters(TargetRelativeLocation2, GetActorRotation(), 0.001);

		GetCharacterMovement()->StopMovementImmediately();
	}
}

void AATHAscila::ExitBrace()
{
	if(ParkourStatus != EParkourStatus::Eps_BracedClimbingOver)
	{
		ExitRMState();
		SetParentStanceStatus(EParentStance::Eps_InAir);
		SetParkourStatus(EParkourStatus::Eps_NA);
	}
}

void AATHAscila::BracedClimbLedge()
{
	if(ParkourStatus != EParkourStatus::Eps_BracedClimbingOver)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		SetParkourStatus(EParkourStatus::Eps_BracedClimbingOver);
	}
}

void AATHAscila::EnterRMState()
{
	// Add a blend float here so u can change the float based on parent states
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
}
void AATHAscila::ExitRMState()
{
	if (ParkourStatus == EParkourStatus::Eps_BracedClimbingOver)
	{
		SetParentStanceStatus(EParentStance::Eps_Standing);
		SetParkourStatus(EParkourStatus::Eps_NA);
		SetStanceStatus(EStanceStatus::Ess_StandJogging);
	}
	GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, 0.0035, true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

}

// State Getters
bool AATHAscila::GetCanGrab()
{
	return bCanGrab;
}

	#pragma endregion