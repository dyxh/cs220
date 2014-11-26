// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PlatformerCharacter.h"


APlatformerCharacter::APlatformerCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void APlatformerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlatformerCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	InputComponent->BindAxis("MoveRight", this, &APlatformerCharacter::MoveRight);
}

void APlatformerCharacter::MoveRight(float MovementValue)
{
	if ((Controller != NULL) && (MovementValue != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, MovementValue);
	}
}