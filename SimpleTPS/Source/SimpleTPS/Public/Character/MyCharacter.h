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
	class USpringArmComponent* cameraArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* muzzlePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float runMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sprintMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float aimSpeed;

public:
	float CalcForwardToInputDegree(FVector lastInputVector);
	void MoveForward(float InputValue);
	void MoveRight(float InputValue);
	void LookUp(float InputValue);
	void Jump();
	void Fire();
	void IdleToAim();
	void AimToIdle();
	void IdleToCrouch();
	void CrouchToIdle();
	void Sprint();
	void UnSprint();
	void Interaction();

private:
	const float idleCameraArmLength = 225.f;
	const float aimCameraArmLength = 45.f;
	float moveSpeed;
};
