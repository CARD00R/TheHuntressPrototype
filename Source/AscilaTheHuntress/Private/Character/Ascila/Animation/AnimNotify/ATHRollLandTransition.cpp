// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimNotify/ATHRollLandTransition.h"
#include "Character/Ascila/ATHAscila.h"
#include "Engine.h"

void UATHRollLandTransition::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->SetParentStanceStatus(EParentStance::Eps_Standing);
			AscilaCharacter->SetStanceStatus(EStanceStatus::Ess_StandIdling);
		}

	}
	
}

