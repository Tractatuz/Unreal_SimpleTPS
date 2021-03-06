// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"
#include "Character/MyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetMathLibrary.h"

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

	moveSpeed = walkMoveSpeed;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (animInstance != nullptr)
	{
		animInstance->velocity = FVector(GetVelocity().X, GetVelocity().Y, 0).Size();
		animInstance->direction = CalcDirection();
		animInstance->isJump = GetCharacterMovement()->IsFalling();
		animInstance->isCrouch = GetCharacterMovement()->IsCrouching();
		animInstance->pitch = cameraArm->GetRelativeRotation().Pitch;

		if (cameraArm != nullptr)
		{
			if (animInstance->isAim)
			{
				if (cameraArm->TargetArmLength < aimCameraArmLength 
					|| abs(cameraArm->TargetArmLength - aimCameraArmLength) < aimCameraSpeed * DeltaTime)
				{
					cameraArm->TargetArmLength = aimCameraArmLength;
				}
				else
				{
					cameraArm->TargetArmLength -= aimCameraSpeed * DeltaTime;
				}
			}
			else
			{
				if (cameraArm->TargetArmLength > idleCameraArmLength 
					|| abs(cameraArm->TargetArmLength - idleCameraArmLength) < aimCameraSpeed * DeltaTime)
				{
					cameraArm->TargetArmLength = idleCameraArmLength;
				}
				else
				{
					cameraArm->TargetArmLength += aimCameraSpeed * DeltaTime;
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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMyCharacter::IdleToAim);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMyCharacter::AimToIdle);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::IdleToCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacter::CrouchToIdle);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::UnSprint);
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AMyCharacter::Interaction);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::LookUp);
}

void AMyCharacter::MoveForward(float InputValue)
{
	FRotator rotation = Controller->GetControlRotation();
	FRotator yawRotation(0, rotation.Yaw, 0);
	FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(direction, InputValue * moveSpeed);
}

void AMyCharacter::MoveRight(float InputValue)
{
	FRotator rotation = Controller->GetControlRotation();
	FRotator yawRotation(0, rotation.Yaw, 0);
	FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(direction, InputValue * moveSpeed);
}

void AMyCharacter::LookUp(float InputValue)
{
	float pitch = cameraArm->GetRelativeRotation().Pitch;
	if ((InputValue > 0 && pitch < 45.f)
		|| (InputValue < 0 && pitch > -45.f))
	{
		FRotator PitchRotation(InputValue, 0, 0);
		cameraArm->AddRelativeRotation(PitchRotation);
	}
}

void AMyCharacter::Jump()
{
	GetCharacterMovement()->DoJump(true);
}

void AMyCharacter::Fire()
{
	UBlueprintGeneratedClass* bullet = LoadObject<UBlueprintGeneratedClass>(nullptr, *FString("/Game/01_BP/Object/BP_Bullet.BP_Bullet_C"));
	if (bullet)
	{
		if (muzzlePoint)
		{
			FRotator spawnRotation = muzzlePoint->GetComponentRotation();
			FVector spawnLocation = muzzlePoint->GetComponentLocation();

			if (GetWorld())
			{
				GetWorld()->SpawnActor<AActor>(bullet, spawnLocation, spawnRotation);
			}
		}
	}
}

void AMyCharacter::IdleToAim()
{
	if (animInstance != nullptr)
	{
		if (animInstance->isSprint)
		{
			return;
		}

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

void AMyCharacter::IdleToCrouch()
{
	moveSpeed = crouchMoveSpeed;
	UnSprint();
	Crouch();
}

void AMyCharacter::CrouchToIdle()
{
	moveSpeed = walkMoveSpeed;
	UnCrouch();
}

void AMyCharacter::Sprint()
{
	if (animInstance != nullptr)
	{
		if (animInstance->isCrouch)
		{
			return;
		}

		animInstance->isSprint = true;
		moveSpeed = sprintMoveSpeed;
	}
}

void AMyCharacter::UnSprint()
{
	if (animInstance != nullptr)
	{
		if (animInstance->isSprint)
		{
			animInstance->isSprint = false;
			moveSpeed = walkMoveSpeed;
		}
	}
}

void AMyCharacter::Interaction()
{
}

float AMyCharacter::CalcDirection()
{
	FVector velocityXY = FVector(GetVelocity().X, GetVelocity().Y, 0);
	FRotator rotation = UKismetMathLibrary::MakeRotFromX(velocityXY);

	rotation += GetActorRotation().GetInverse();

	return rotation.Yaw;
}