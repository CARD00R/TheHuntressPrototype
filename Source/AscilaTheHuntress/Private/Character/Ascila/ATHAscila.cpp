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
	PlayerInputComponent->BindAction("Walk", IE_Pressed, this, &AATHAscila::RequestWalk);
	PlayerInputComponent->BindAction("Walk", IE_Released, this, &AATHAscila::WalkReleased);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AATHAscila::RequestSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AATHAscila::SprintReleased);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AATHAscila::RequestCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AATHAscila::AscilaCrouchRelease);
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
			if(StanceStatus == EStanceStatus::Ess_SSprinting)
			{
				
			}
			else if(StanceStatus == EStanceStatus::Ess_SWalking)
			{
				
			}
			else
			{
				SetStanceStatus(EStanceStatus::Ess_SJogging);
			}
		}
		else if(ParentStance == EParentStance::Eps_Crouching)
		{
			SetStanceStatus(EStanceStatus::Ess_CWalking);
		}
	}
	// Not pressing forward
	else
	{
		// not moving forward and right
		if (!bIsMovingForward && !bIsMovingRight)
		{
			// Is idle check animation
			IdleCheck();
		}
		else
		{
			bIdleCheck = true;
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
			SetStanceStatus(EStanceStatus::Ess_SJogging);
		}
		else if (ParentStance == EParentStance::Eps_Crouching)
		{
			SetStanceStatus(EStanceStatus::Ess_CWalking);
		}
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
void AATHAscila::RequestWalk()
{
	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (StanceStatus != EStanceStatus::Ess_SWalking)
		{
			if (StanceStatus != EStanceStatus::Ess_CIdling || GetStanceStatus() != EStanceStatus::Ess_CWalking)
			{
				if(ParentStance != EParentStance::Eps_Rolling)
				{
					Walk();
				}
				else
				{
					SetRequestedStatus(ERequestStance::Ers_SWalking);
				}
			}

		}
	}
}
void AATHAscila::Walk()
{
	SetParentStanceStatus(EParentStance::Eps_Standing);
	SetStanceStatus(EStanceStatus::Ess_SWalking);
	SetRequestedStatus(ERequestStance::Ers_NA);
}
void AATHAscila::WalkReleased()
{
	if(StanceStatus == EStanceStatus::Ess_SWalking)
	{
		if(bIsMovingForward || bIsMovingRight)
		{
			SetStanceStatus(EStanceStatus::Ess_SJogging);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_SIdling);
		}
		
	}
}
void AATHAscila::RequestSprint()
{

	if (ParentStance != EParentStance::Eps_Dead)
	{
		if (StanceStatus != EStanceStatus::Ess_SSprinting)
		{
			if(StanceStatus != EStanceStatus::Ess_SIdling)
			{
				if (ParentStance != EParentStance::Eps_Rolling)
				{
					Sprint();
				}
				else
				{
					SetRequestedStatus(ERequestStance::Ers_SSprinting);
				}
			}
		}
	}
}
void AATHAscila::Sprint()
{
	SetParentStanceStatus(EParentStance::Eps_Standing);
	SetStanceStatus(EStanceStatus::Ess_SSprinting);
	SetRequestedStatus(ERequestStance::Ers_NA);
	UE_LOG(LogTemp, Error, TEXT("SPRINT"));
}
void AATHAscila::SprintReleased()
{
	if(bIsMovingForward)
	{
		SetStanceStatus(EStanceStatus::Ess_SJogging);
	}
	else
	{
		SetStanceStatus(EStanceStatus::Ess_SIdling);
	}

}
void AATHAscila::RequestCrouch()
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
				SetRequestedStatus(ERequestStance::Ers_CWalking);
			}
			else
			{
				SetRequestedStatus(ERequestStance::Ers_CIdling);
			}

		}
	}
}
void AATHAscila::AscilaCrouch()
{
	SetParentStanceStatus(EParentStance::Eps_Crouching);
	
	if(StanceStatus == EStanceStatus::Ess_SIdling)
	{
		SetStanceStatus(EStanceStatus::Ess_CIdling);
	}
	else
	{
		SetStanceStatus(EStanceStatus::Ess_CWalking);
	}

	SetRequestedStatus(ERequestStance::Ers_NA);
}

void AATHAscila::AscilaCrouchRelease()
{
	if(ParentStance == EParentStance::Eps_Crouching)
	{
		if (StanceStatus == EStanceStatus::Ess_CIdling)
		{
			SetStanceStatus(EStanceStatus::Ess_SIdling);
		}
		else
		{
			SetStanceStatus(EStanceStatus::Ess_SJogging);
		}
		
		SetParentStanceStatus(EParentStance::Eps_Standing);
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
	case EStanceStatus::Ess_SIdling:
		SetCharacterSpeed(JogSpeed);
		break;
	case EStanceStatus::Ess_SWalking:
		SetCharacterSpeed(WalkSpeed);
		break;
	case EStanceStatus::Ess_SJogging:
		SetCharacterSpeed(JogSpeed);
		break;
	case EStanceStatus::Ess_SSprinting:
		SetCharacterSpeed(SprintSpeed);
		break;
	case EStanceStatus::Ess_CIdling:
		SetCharacterSpeed(CrouchSpeed);
		break;
	case EStanceStatus::Ess_CWalking:
		SetCharacterSpeed(CrouchSpeed);
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
			SetStanceStatus(EStanceStatus::Ess_SIdling);
			break;
		case EParentStance::Eps_Crouching:
			SetStanceStatus(EStanceStatus::Ess_CIdling);
			break;
		}

		bIdleCheck = false;
	}
}


#pragma endregion 