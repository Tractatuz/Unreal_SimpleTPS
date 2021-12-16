// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class SIMPLETPS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	class UMyAnimInstance* animInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float moveSpeed = 1.f;

public:
	void MoveForward(float InputValue);
	void MoveRight(float InputValue);
	void Jump();
	void Attack();
	void Interaction();

};
