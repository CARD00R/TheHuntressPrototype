// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ATHTogglePCRMRotation.generated.h"

/**
 * 
 */
class AATHAscila;
UCLASS()
class ASCILATHEHUNTRESS_API UATHTogglePCRMRotation : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:
	AATHAscila* AscilaChar;
	float TimerCalls = 0;
	void CallGrabLedge();
	USkeletalMeshComponent* SKMesh;
	FTimerHandle GrabLedgeHandle;
};
