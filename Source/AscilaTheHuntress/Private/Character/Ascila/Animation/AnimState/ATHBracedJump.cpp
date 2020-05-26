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
			APlayerController* AscilaPC = MeshComp->GetWorld()->GetFirstPlayerController();

			if (AscilaPC != nullptr)
			{
				AscilaCharacter->GetController()->GetPawn()->DisableInput(AscilaPC);
				AscilaCharacter->SetButtonEnabled(false);
			}
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
			APlayerController* AscilaPC = MeshComp->GetWorld()->GetFirstPlayerController();

			if (AscilaPC != nullptr)
			{
				AscilaCharacter->GetController()->GetPawn()->EnableInput(AscilaPC);
			}
			AscilaCharacter->SetButtonEnabled(true);
			AscilaCharacter->GrabLedge();
			AscilaCharacter->SetParkourStatus(EParkourStatus::Eps_BracedIdling);

		}
	}
}






