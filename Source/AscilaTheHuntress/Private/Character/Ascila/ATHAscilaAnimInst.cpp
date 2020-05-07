// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/ATHAscilaAnimInst.h"
#include "Character/Ascila/ATHAscila.h"
#include "Kismet/KismetMathLibrary.h"

void UATHAscilaAnimInst::NativeInitializeAnimation()
{
	if (!AscilaPawn)
	{
		AscilaPawn = TryGetPawnOwner();
		//
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
			bIsAiming = AscilaCharacter->GetIsAiming();
			
			//Calculations
			CalculatePitch(DeltaTime);
		}
	}
	else
	{
		AscilaPawn = TryGetPawnOwner();
	}
}

void UATHAscilaAnimInst::CalculatePitch(float DeltaTime)
{
	//Current Rotation
	FRotator Current = FRotator(Pitch, Yaw, 0);
	//The Difference between the control rotation(mouse) and the actor rotation
	FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(AscilaCharacter->GetControlRotation(), AscilaCharacter->GetActorRotation());
	// Constantly reseting the value of the return so it can keep up with mouse movement
	FRotator Return = UKismetMathLibrary::RInterpTo(Current, Delta, DeltaTime, 60);
	// Clamping values to prevent unwanted rotation
	Pitch = FMath::ClampAngle(Return.Pitch, -60, 60);
	Yaw = FMath::ClampAngle(Return.Yaw, -60, 60);
	// Setting character yaw/pitch
	AscilaCharacter->SetPitch(Pitch);
	AscilaCharacter->SetYaw(Yaw);

}
