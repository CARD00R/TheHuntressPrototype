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

	//Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//CameraComp->bUsePawnControlRotation = false;

	//Capsule Component
	GetCapsuleComponent()->SetCapsuleRadius(StandCapsuleRadius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(StandCapsuleHalfHeight);

	// Movement Component
	CharMovementComp = GetCharacterMovement();

	CharMovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	CharMovementComp->GetNavAgentPropertiesRef().bCanJump = true;
	CharMovementComp->MaxWalkSpeed = JogSpeed;

	CharMovementComp->bUseControllerDesiredRotation = false;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AATHAscila::BeginPlay()
{
	Super::BeginPlay();
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
	
	//PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AATHAscila::RequestDrawBow);
}

	#pragma region  Input

void AATHAscila::MoveForward(float value)
{
	// Pressing forward...
	if(value != 0)
	{
		bIsMovingForward = true;

		if(ParentStance == EParentStance::Eps_Standing)
		{
			if(StanceStatus == EStanceStatus::Ess_StandSprinting && value > 0)
			{
				
			}
			
			else
			{
				SetStanceStatus(EStanceStatus::Ess_StandJogging);
			}
		}
		else if(ParentStance == EParentStance::Eps_Crouching)
		{
			if(StanceStatus == EStanceStatus::Ess_CrouchSprinting && value > 0)
			{
				
			}
			else
			{
				SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
			}
		}
		
		bIdleCheck = true;
	}
	// Not pressing forward
	else
	{
		bIsMovingForward = false;
		
		// not moving forward and right
		if (!bIsMovingForward && !bIsMovingRight)
		{
			// Is idle check animation
			IdleCheck();
		}
		else
		{
			
		}
	}
	
	AddMovementInput(GetActorForwardVector()*value);
}
void AATHAscila::MoveRight(float value)
{
	if (value != 0)
	{
		bIsMovingRight = true;

		if (ParentStance == EParentStance::Eps_Standing)
		{
			SetStanceStatus(EStanceStatus::Ess_StandJogging);
		}
		else if (ParentStance == EParentStance::Eps_Crouching)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
		}

		bIdleCheck = true;
	}
	else
	{
		bIsMovingRight = false;
	}
	
	AddMovementInput(GetActorRightVector()* value);
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
						if (bIsMovingForward && !bIsMovingRight)
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
		SetRequestedStatus(ERequestStance::Ers_NA);
	}
	
}
void AATHAscila::SprintReleased()
{
	if(StanceStatus == EStanceStatus::Ess_StandSprinting)
	{
		if (bIsMovingForward)
		{
			SetStanceStatus(EStanceStatus::Ess_StandJogging);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_StandIdling);
		}

	}
	else if(StanceStatus == EStanceStatus::Ess_CrouchSprinting)
	{
		if (bIsMovingForward)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchIdling);
		}
	}
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
					AscilaUnCrouch();
				}
				else
				{
					if (bIsMovingRight || bIsMovingForward)
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
				AscilaCrouch();
			}
			else
			{
				if (bIsMovingRight || bIsMovingForward)
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
			
			//GetMesh()->SetRelativeLocation(CrouchMeshInitialiseLocation);
			//GetCapsuleComponent()->SetCapsuleRadius(CrouchCapsuleRadius);
			//GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchCapsuleHalfHeight);
			TargetMeshLocation = CrouchMeshInitialiseLocation;
			TargetCapsuleRadius = CrouchCapsuleRadius;
			TargetCapsuleHalfHeight = CrouchCapsuleHalfHeight;
			GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
		}
		else if (StanceStatus == EStanceStatus::Ess_StandJogging)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
			//GetMesh()->SetRelativeLocation(CrouchMeshInitialiseLocation);
			//GetCapsuleComponent()->SetCapsuleRadius(CrouchCapsuleRadius);
			//GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchCapsuleHalfHeight);
			TargetMeshLocation = CrouchMeshInitialiseLocation;
			TargetCapsuleRadius = CrouchCapsuleRadius;
			TargetCapsuleHalfHeight = CrouchCapsuleHalfHeight;
			GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchSprinting);
			//GetMesh()->SetRelativeLocation(CrouchSprintMeshInitialiseLocation);
			//GetCapsuleComponent()->SetCapsuleRadius(CrouchSprintCapsuleRadius);
			//GetCapsuleComponent()->SetCapsuleHalfHeight(CrouchSprintCapsuleHalfHeight);
			TargetMeshLocation = CrouchSprintMeshInitialiseLocation;
			TargetCapsuleRadius = CrouchSprintCapsuleRadius;
			TargetCapsuleHalfHeight = CrouchSprintCapsuleHalfHeight;
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
		//GetMesh()->SetRelativeLocation(StandMeshInitialiseLocation);
		//GetCapsuleComponent()->SetCapsuleRadius(StandCapsuleRadius);
		//GetCapsuleComponent()->SetCapsuleHalfHeight(StandCapsuleHalfHeight);
		TargetMeshLocation = StandMeshInitialiseLocation;
		TargetCapsuleRadius = StandCapsuleRadius;
		TargetCapsuleHalfHeight = StandCapsuleHalfHeight;
		GetWorldTimerManager().SetTimer(CapsuleMeshProprtiesChangeTimer, this, &AATHAscila::CapsuleMeshPropertiesChange, CapsuleMeshAlpha, true);
	}
	
	SetRequestedStatus(ERequestStance::Ers_NA);
}

void AATHAscila::CapsuleMeshPropertiesChange()
{
	//GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()
	//GetCapsuleComponent()->GetUnscaledCapsuleRadius()
	//GetMesh()->GetRelativeLocation();
	bool IsEqualHalfeight = false;
	bool IsEqualRadius = false;
	bool IsLocation = false;

	if(!(FMath::IsNearlyEqual(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetCapsuleHalfHeight,CapsuleTolerance)))
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetCapsuleHalfHeight, HeightAlpha));
	}
	else
	{
		IsEqualHalfeight = true;
	}
	if(!(FMath::IsNearlyEqual(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), TargetCapsuleRadius, CapsuleTolerance)))
	{
		GetCapsuleComponent()->SetCapsuleRadius(FMath::Lerp(GetCapsuleComponent()->GetUnscaledCapsuleRadius(), TargetCapsuleRadius, RadiusAlpha));
	}
	else
	{
		IsEqualRadius = true;
	}	
	if(FVector::Dist(GetMesh()->GetRelativeLocation(), TargetMeshLocation) > CapsuleTolerance)
	{
		GetMesh()->SetRelativeLocation(FMath::Lerp(GetMesh()->GetRelativeLocation(), TargetMeshLocation, LocationAlpha));
	}
	else
	{
		IsLocation = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Timer"));

	if(IsEqualHalfeight && IsEqualRadius && IsLocation)
	{
		UE_LOG(LogTemp, Error, TEXT("STOP"));
		GetWorldTimerManager().ClearTimer(CapsuleMeshProprtiesChangeTimer);
	}
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
		break;
	case EStanceStatus::Ess_StandSprinting:
		SetCharacterSpeed(SprintSpeed);
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
	}

	ChangeRotationRate();
}
EStanceStatus AATHAscila::GetStanceStatus()
{
	return StanceStatus;
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

void AATHAscila::SetCharacterSpeed(float Speed)
{
	CharMovementComp->MaxWalkSpeed = Speed;
}
void AATHAscila::ChangeRotationRate()
{
	if(StanceStatus == EStanceStatus::Ess_StandSprinting)
	{
		bUseControllerRotationYaw = false;
		CharMovementComp->bUseControllerDesiredRotation = true;
		CharMovementComp->RotationRate = SprintingRotationRate;
	}
	else
	{
		bUseControllerRotationYaw = true;
		CharMovementComp->bUseControllerDesiredRotation = false;
	}
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
		AimReadyAlpha = 0.003f;
		PlayAnimMontage(DrawArrowMontage, 1.0f, NAME_None);
	}
	else
	{
		AimReadyAlpha = 0.001f;
	}
	
	GetWorldTimerManager().SetTimer(AimingReadyHandle, this, &AATHAscila::SetAimReadyValue, AimReadyAlpha, true);

	//SpringArmComp->SocketOffset = SpringCompSocketAimOffset;
}
void AATHAscila::AimOut()
{
	bIsAiming = false;
	SetBowStatus(EBowStatus::Ebs_NA);
	GetWorldTimerManager().SetTimer(AimingReadyHandle, this, &AATHAscila::SetAimReadyValue, AimReadyAlpha, true);
	//SpringArmComp->SocketOffset = SpringCompSocketDefaultOffset;
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
}
	#pragma endregion 

	#pragma  region Utilities

void AATHAscila::ChangeCameraProperties()
{
	//bool 
}

	#pragma endregion