// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ATHBracedJump.generated.h"

/**
 *
 */
UCLASS()
class ASCILATHEHUNTRESS_API UATHBracedJump : public UAnimNotifyState
{
	GENERATED_BODY()


		virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
