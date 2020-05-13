// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Ascila/ATHAscilaPC.h"


void AATHAscilaPC::UpdateRotation(float DeltaTime)
{
	if(!bisRootMotionRotating)
	{
		Super::UpdateRotation(DeltaTime);
	}
}

void AATHAscilaPC::SetIsRootMotionRotating(bool IsRMRotating)
{
	bisRootMotionRotating = IsRMRotating;
}
