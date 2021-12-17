// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "Character/MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->JumpZVelocity = 350.0f;

	animInstance = ::Cast<UMyAnimInstance>(this->GetMesh()->GetAnimInstance());

	cameraArm = ::Cast<USpringArmComponent>(this->GetComponentByClass(USpringArmComponent::StaticClass()));
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (animInstance != nullptr)
	{
		animInstance->isJump = GetCharacterMovement()->IsFalling();
		animInstance->velocity = GetVelocity().Size();

		if (cameraArm != nullptr)
		{
			if (animInstance->isAim)
			{
				if (cameraArm->TargetArmLength < aimCameraArmLength 
					|| abs(cameraArm->TargetArmLength - aimCameraArmLength) < aimSpeed * DeltaTime)
				{
					cameraArm->TargetArmLength = aimCameraArmLength;
				}
				else
				{
					cameraArm->TargetArmLength -= aimSpeed * DeltaTime;
				}
			}
			else
			{
				if (cameraArm->TargetArmLength > idleCameraArmLength 
					|| abs(cameraArm->TargetArmLength - idleCameraArmLength) < aimSpeed * DeltaTime)
				{
					cameraArm->TargetArmLength = idleCameraArmLength;
				}
				else
				{
					cameraArm->TargetArmLength += aimSpeed * DeltaTime;
				}
			}
		}
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMyCharacter::Interaction);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMyCharacter::IdleToAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMyCharacter::AimToIdle);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}

void AMyCharacter::MoveForward(float InputValue)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, InputValue * moveSpeed);
}

void AMyCharacter::MoveRight(float InputValue)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, InputValue * moveSpeed);
}

void AMyCharacter::Jump()
{
	GetCharacterMovement()->DoJump(true);
}

void AMyCharacter::IdleToAim()
{
	if (animInstance != nullptr)
	{
		animInstance->isAim = true;
	}
}

void AMyCharacter::AimToIdle()
{
	if (animInstance != nullptr)
	{
		animInstance->isAim = false;
	}
}

void AMyCharacter::Interaction()
{
}
