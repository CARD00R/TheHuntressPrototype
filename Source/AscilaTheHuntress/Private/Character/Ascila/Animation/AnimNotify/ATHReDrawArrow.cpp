// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/Animation/AnimNotify/ATHReDrawArrow.h"
#include "Character/Ascila/ATHAscila.h"
#include "Engine.h"

void UATHReDrawArrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		AATHAscila* AscilaCharacter = Cast<AATHAscila>(MeshComp->GetOwner());
		if(AscilaCharacter != nullptr)
		{
			AscilaCharacter->SetArrowDrawnVariable(false);
			AscilaCharacter->SetBowStatus(EBowStatus::Ebs_NA);
			if(AscilaCharacter->GetIsAiming())
			{
				AscilaCharacter->ReDrawArrow();
			}

			UE_LOG(LogTemp, Error, TEXT("DRAW ARROW!"));
		}
	
	}
}
