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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	EStanceStatus StanceStatus;

protected:
	
	UFUNCTION(BlueprintCallable, Category = "Animation Properties")
	void UpdateAnimationProperties(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	APawn* AscilaPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	AATHAscila* AscilaCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	float MovementSpeedX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	float MovementSpeedY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement Properties")
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Falling Properties")
	float VerticalVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming Properties")
	float Pitch;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming Properties")
	float Yaw;
	void CalculatePitch(float DeltaTime);

	
public:
	
	virtual void NativeInitializeAnimation() override;
};
