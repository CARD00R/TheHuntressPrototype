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
		}
	}
	else
	{
		AscilaPawn = TryGetPawnOwner();
	}
}