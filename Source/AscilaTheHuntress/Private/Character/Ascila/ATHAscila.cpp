// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/ATHAscila.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AATHAscila::AATHAscila()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetWorldLocationAndRotation(MeshInitialiseLocation, MeshInitialiseRotation);

	//Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	//SpringArmComp->SetWorldLocation(SpringArmInitialiseLocation);
	//SpringArmComp->SocketOffset = SpringArmInitialiseSocketOffset;

	//Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	//CameraComp->bUsePawnControlRotation = false;

	//Capsule Component
	GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);
	GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);

	CharMovementComp = GetCharacterMovement();

	CharMovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	CharMovementComp->GetNavAgentPropertiesRef().bCanJump = true;
	CharMovementComp->MaxWalkSpeed = JogSpeed;
}

// Called when the game starts or when spawned
void AATHAscila::BeginPlay()
{
	Super::BeginPlay();
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
	AddControllerPitchInput(value);
}
void AATHAscila::LookRight(float value)
{
	AddControllerYawInput(value);
}
void AATHAscila::RequestSprint()
{

	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (StanceStatus != EStanceStatus::Ess_StandSprinting || StanceStatus != EStanceStatus::Ess_CrouchSprinting)
		{
			if(StanceStatus != EStanceStatus::Ess_StandIdling || StanceStatus != EStanceStatus::Ess_CrouchIdling)
			{
				if (ParentStance != EParentStance::Eps_Rolling)
				{
					if(bIsMovingForward && !bIsMovingRight)
					{
						Sprint();
					}
				}
				else
				{
					if(ParentStance == EParentStance::Eps_Standing)
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
		}
		else if (StanceStatus == EStanceStatus::Ess_StandJogging)
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchWalking);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_CrouchSprinting);
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
	}
	
	SetRequestedStatus(ERequestStance::Ers_NA);
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
		SetCharacterSpeed(JogSpeed);
		break;
	case EStanceStatus::Ess_StandJogging:
		SetCharacterSpeed(JogSpeed);
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
}

EStanceStatus AATHAscila::GetStanceStatus()
{
	return StanceStatus;
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