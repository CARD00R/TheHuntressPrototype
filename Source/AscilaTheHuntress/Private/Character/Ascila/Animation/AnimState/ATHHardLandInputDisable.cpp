// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimState/ATHHardLandInputDisable.h"
#include "Character/Ascila/ATHAscila.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

void UATHHardLandInputDisable::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->GetController()->GetPawn()->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
		}
	}
}

void UATHHardLandInputDisable::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->GetController()->GetPawn()->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
			AscilaCharacter->SetParentStanceStatus(EParentStance::Eps_Standing);
			AscilaCharacter->SetStanceStatus(EStanceStatus::Ess_StandIdling);
		}
	}
}
