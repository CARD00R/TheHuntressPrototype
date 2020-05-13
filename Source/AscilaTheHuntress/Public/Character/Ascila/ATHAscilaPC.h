// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ATHAscilaPC.generated.h"

/**
 * 
 */
class AATHAscila;

UCLASS()
class ASCILATHEHUNTRESS_API AATHAscilaPC : public APlayerController
{
	GENERATED_BODY()
protected:

	// When player is meant to be rotating through root motion
	bool bisRootMotionRotating = false;
public:
	
	virtual void UpdateRotation(float DeltaTime) override;
	
	void SetIsRootMotionRotating(bool IsRMRotating);
};
