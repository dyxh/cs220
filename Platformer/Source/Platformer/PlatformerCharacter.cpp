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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("We are using a platformer character"));
	}
}

void APlatformerCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// bind axis
	InputComponent->BindAxis("MoveRight", this, &APlatformerCharacter::MoveRight);
}

void APlatformerCharacter::MoveRight(float val)
{
	if ((Controller != NULL) && (val != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, val);
	}
}