// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ATHAscila.h"
#include "ATHAscilaAnimInst.generated.h"

/**
 * 
 */

class APawn;
class AATHAscila;

UCLASS()
class ASCILATHEHUNTRESS_API UATHAscilaAnimInst : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
	EStanceStatus StanceStatus;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
	EParentStance ParentStance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "States")
	EParkourStatus ParkourStatus;

protected:
	
	UFUNCTION(BlueprintCallable, Category = "Animation Properties")
	void UpdateAnimationProperties(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	APawn* AscilaPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	AATHAscila* AscilaCharacter;

	#pragma region Movement Properties
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	float MovementSpeedX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	float MovementSpeedY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	float MovementSpeed;

	bool ShouldCloseWindow = false;
	void CloseJumpWindow();
	void CloseJumpWindowReset();

	#pragma endregion
	
	#pragma region Aiming Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming Properties")
	float Pitch;
	void CalculatePitch(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming Properties")
	float Yaw;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming Properties")
	bool bIsAiming;
	#pragma endregion 

	#pragma region Falling & Landing Properties
	
	void DetermineVerticalVelocityProperties();

	UPROPERTY(BlueprintReadOnly, Category = "Falling Properties")
	float VerticalVelocity;
	
	UPROPERTY(BlueprintReadOnly, Category = "Falling Properties")
	bool bNeedsToLand;

	float StoredZLocation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Falling Properties")
	float FallHeight;

	float StoredFallHeight;

	float FallHeightStartingZ;
	
	bool bShouldResetFallHeight = false;
	void ResetFallHeight();
	FTimerHandle ResetFallHeightHandle;
	void FallHeightVarSetter();

	bool bShouldLandRoll = false;
	bool bShouldHardLand = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Falling Properties")
	float LandAlpha = 0.0f;

	#pragma endregion

	

	#pragma region Parkour
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parkour Properties")
	bool bCanBracedTurnLeft = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Parkour Properties")
	bool bCanBracedTurnRight = false;

	#pragma endregion  

public:
	
	virtual void NativeInitializeAnimation() override;
};
