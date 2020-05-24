// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ATHChangeCapsuleProperties.generated.h"

/**
 * 
 */
UCLASS()
class ASCILATHEHUNTRESS_API UATHChangeCapsuleProperties : public UAnimNotify
{
	GENERATED_BODY()

public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
