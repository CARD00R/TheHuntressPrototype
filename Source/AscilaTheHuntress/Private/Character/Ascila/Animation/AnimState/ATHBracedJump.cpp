// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimState/ATHBracedJump.h"
#include "Character/Ascila/ATHAscila.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

void UATHBracedJump::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->SetButtonEnabled(false);
		}
	}
}

void UATHBracedJump::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->SetButtonEnabled(true);
			AscilaCharacter->SetParkourStatus(EParkourStatus::Eps_NA);
			AscilaCharacter->GrabLedge();
			//APlayerController* AscilaPC = MeshComp->GetWorld()->GetFirstPlayerController();
		}
	}
}






