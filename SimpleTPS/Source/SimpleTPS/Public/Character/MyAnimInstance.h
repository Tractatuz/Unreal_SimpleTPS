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
	bool isAttack = false;
};