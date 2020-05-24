// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimNotify/ATHExitRMState.h"
#include "Character/Ascila/ATHAscila.h"

void UATHExitRMState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			AscilaCharacter->ExitRMState();
		}
	}
}
