// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLETPS_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float velocity = 0.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isAim = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isJump = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isSprint = false;
};