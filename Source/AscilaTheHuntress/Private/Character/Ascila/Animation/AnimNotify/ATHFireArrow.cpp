// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimNotify/ATHFireArrow.h"
#include "Character/Ascila/ATHAscila.h"
#include "Weapon/BowAndArrow/ATHBow.h"
#include "Engine.h"

void UATHFireArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if (AscilaCharacter != nullptr)
		{
			if(AscilaCharacter->EquippedBow != nullptr)
			{
				AscilaCharacter->EquippedBow->FireArrow();
				UE_LOG(LogTemp, Error, TEXT("FIIIIIIRE!"));
			}
		}
	}
}