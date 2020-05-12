// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/ATHAscilaAnimInst.h"
#include "Character/Ascila/ATHAscila.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

void UATHAscilaAnimInst::NativeInitializeAnimation()
{
	if (!AscilaPawn)
	{
		AscilaPawn = TryGetPawnOwner();
	}
}

void UATHAscilaAnimInst::UpdateAnimationProperties(float DeltaTime)
{
	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);
	
	if (AscilaPawn)
	{
		//Calculate Movement Speed
		FVector Speed = AscilaPawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0);
		FTransform MeshTransform = GetSkelMeshComponent()->GetComponentTransform();

		// Fly/Fall Speed
		VerticalVelocity = AscilaPawn->GetVelocity().Z;

		// Obtain horizontal Speed (Inversed [-1])
		MovementSpeedX = (UKismetMathLibrary::InverseTransformDirection(MeshTransform, Speed).X)*-1.0f;
		// Obtain Vertical Speed
		MovementSpeedY = UKismetMathLibrary::InverseTransformDirection(MeshTransform, Speed).Y;
		// Obtain Directional Speed
		MovementSpeed = LateralSpeed.Size();

		AscilaCharacter = Cast<AATHAscila>(AscilaPawn);
		
		// If character exists...		
		if (AscilaCharacter != nullptr)
		{
			//Sets AnimInst StanceStatus to the Character's
			StanceStatus = AscilaCharacter->GetStanceStatus();
			ParentStance = AscilaCharacter->GetParentStanceStatus();
			bIsAiming = AscilaCharacter->GetIsAiming();
			bNeedsToLand = AscilaCharacter->GetNeedsToLand();
			AscilaCharacter->SetShouldRollLand(bShouldLandRoll);
			AscilaCharacter->SetShouldHardLand(bShouldHardLand);
			
			//Calculations
			CalculatePitch(DeltaTime);
			DetermineVerticalVelocityProperties();
		}
	}
	else
	{
		AscilaPawn = TryGetPawnOwner();
		UE_LOG(LogTemp, Error, TEXT("No Character Found"));
	}
}

void UATHAscilaAnimInst::CalculatePitch(float DeltaTime)
{
	// Get Pitch Constraints
	float MinPitch = AscilaCharacter->GetMinPitch();
	float MaxPitch = AscilaCharacter->GetMaxPitch();
	//Current Rotation
	FRotator Current = FRotator(Pitch, Yaw, 0);
	//The Difference between the control rotation(mouse) and the actor rotation
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(AscilaCharacter->GetControlRotation(), AscilaCharacter->GetActorRotation());
	// Constantly reseting the value of the return so it can keep up with mouse movement
	FRotator Return = UKismetMathLibrary::RInterpTo(Current, Delta, DeltaTime, 60);
	// Clamping values to prevent unwanted rotation
	Pitch = FMath::ClampAngle(Return.Pitch, MinPitch, MaxPitch);
	Yaw = FMath::ClampAngle(Return.Yaw, MinPitch, MaxPitch);
	// Setting character yaw/pitch
	AscilaCharacter->SetPitch(Pitch);
	AscilaCharacter->SetYaw(Yaw);
}

void UATHAscilaAnimInst::DetermineVerticalVelocityProperties()
{
	StoredZLocation = AscilaCharacter->GetActorLocation().Z;

	// Jumping or OnLand
	if(VerticalVelocity >= 0)
	{
		if(VerticalVelocity == 0)
		{
			// On Land
			GetWorld()->GetTimerManager().SetTimer(ResetFallHeightHandle, this, &UATHAscilaAnimInst::ResetFallHeight, 0.25f, false);
			AscilaCharacter->SetJumpWindowT();
			CloseJumpWindowReset();
		}
		else
		{
			AscilaCharacter->SetParentStanceStatus(EParentStance::Eps_Standing);
			AscilaCharacter->SetJumpWindowF(false);
		}

		FallHeightStartingZ = StoredZLocation;	
	}
	// Falling
	else
	{
		if(GetWorld()->GetTimerManager().IsTimerActive(ResetFallHeightHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(ResetFallHeightHandle);
			ResetFallHeight();
		}
		AscilaCharacter->SetParentStanceStatus(EParentStance::Eps_InAir);
		// Adjust Capsule size maybe?
		AscilaCharacter->SetStanceStatus(EStanceStatus::Ess_InAirJogFalling);
		
		CloseJumpWindow();
		FallHeightVarSetter();
		AscilaCharacter->SetNeedsToLandT();
		FallHeight = FallHeightStartingZ - StoredZLocation;
	}
}

void UATHAscilaAnimInst::ResetFallHeight()
{
	FallHeight = 0;
}

// Determine the additive land animation's alpha
void UATHAscilaAnimInst::FallHeightVarSetter()
{
	if (FallHeight <= 100)
	{
		LandAlpha = 0.5f;
		bShouldLandRoll = false;
		bShouldHardLand = false;
	}
	else if (FallHeight > 100 && FallHeight <= 250)
	{
		LandAlpha = 0.5f;
		bShouldLandRoll = false;
		bShouldHardLand = false;
	}
	else if (FallHeight > 250 && FallHeight <= 375)
	{
		LandAlpha = 0.6f;
		bShouldLandRoll = false;
		bShouldHardLand = false;
	}
	else if (FallHeight > 375 && FallHeight <= 600)
	{
		LandAlpha = 0.7f;
		bShouldLandRoll = false;
		bShouldHardLand = false;
	}
	else if (FallHeight > 600 && FallHeight <= 700)
	{
		LandAlpha = 0.8f;
		bShouldLandRoll = false;
		bShouldHardLand = false;
	}
	else
	{
		if (MovementSpeed > 700)
		{
			bShouldLandRoll = true;
		}
		else
		{
			bShouldHardLand = true;
		}
	}
}

void UATHAscilaAnimInst::CloseJumpWindow()
{
	if(ShouldCloseWindow)
	{
		AscilaCharacter->SetJumpWindowF(true);
		
		ShouldCloseWindow = false;
	}
}

void UATHAscilaAnimInst::CloseJumpWindowReset()
{
	ShouldCloseWindow = true;
}
