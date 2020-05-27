// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimState/ATHTogglePCRMRotation.h"
#include "Character/Ascila/ATHAscila.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
//#inlcude "Actor.h"

void UATHTogglePCRMRotation::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->SetButtonEnabled(false);
			AscilaCharacter->SetPCRootMotionRotation(true);
		}
	}
}

void UATHTogglePCRMRotation::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		AscilaChar = AscilaCharacter;
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->SetButtonEnabled(true);
			AscilaCharacter->SetPCRootMotionRotation(false);
			AscilaCharacter->SetParkourStatus(EParkourStatus::Eps_NA);
			AscilaCharacter->GrabLedge();
			//SKMesh = MeshComp;
			
			//MeshComp->GetWorld()->GetTimerManager().SetTimer(GrabLedgeHandle, this, &UATHTogglePCRMRotation::CallGrabLedge, 0.01, true);
		}
	}
}

void UATHTogglePCRMRotation::CallGrabLedge()
{
	if(TimerCalls < 50)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(AscilaChar);
		if (AscilaChar)
		{
			AscilaCharacter->GrabLedge();
			UE_LOG(LogTemp, Error, TEXT("MEGA GAYYYYY"));
			TimerCalls++;
		}
	}
	else
	{
		SKMesh->GetWorld()->GetTimerManager().ClearTimer(GrabLedgeHandle);
		UE_LOG(LogTemp, Error, TEXT("CLEAAAAAAAAAAAAAAAAAAAR"));
		TimerCalls = 0;
		
	}
}