// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PlatformerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API APlatformerCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	virtual void BeginPlay() override;
	

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Handles left/right movement
	UFUNCTION()
	void MoveRight(float val);
};
