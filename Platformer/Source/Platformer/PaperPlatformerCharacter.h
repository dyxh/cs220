// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "PaperPlatformerCharacter.generated.h"


/**
 * Enumeration of movement states the character can be in
 */
UENUM(BlueprintType)
namespace EMoveState
{
	enum State
	{
		Idle		UMETA(DisplayName = "Idle"),
		Run			UMETA(DisplayName = "Run"),
	};
}

/**
 * Enumeration of battle states the character can be in
 */
UENUM(BlueprintType)
namespace EBattleState
{
	enum State
	{
		Idle		UMETA(DisplayName = "Idle"),
		Shield		UMETA(DisplayName = "Shield"),
		Flinch		UMETA(DisplayName = "Flinch"),
		Attack		UMETA(DisplayName = "Attack"),
	};
}

/**
 * 
 */
UCLASS()
class PLATFORMER_API APaperPlatformerCharacter : public APaperCharacter
{
	GENERATED_UCLASS_BODY()

	// 2d Camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> SideViewCameraComponent;

	// Boom connecting camera to player position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;
    
    // Damage Collision Box
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage)
    TSubobjectPtr<class UBoxComponent> DamageBox;
	
	// Movement State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<EMoveState::State> MoveState;

	// Battle State
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<EBattleState::State> BattleState;

	// Current health of character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float Health;

	// Max possible health of character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float MaxHealth;

	// Current Stamina of Chracter
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float Stamina;

	// Max possible Stamina of Character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float MaxStamina;

	// Stamina regeneration rate
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaminaRegen;

	// Health Regeneration rate
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float HealthRegen;

	// override Tick
	virtual void Tick(float DeltaSeconds) override;

protected:
	// sets up controller interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Handles left/right movement
	UFUNCTION()
	void MoveRight(float val);

	// Handles jump start
	UFUNCTION()
	void OnStartJump();

	// Handles left/right movement
	UFUNCTION()
	void OnStopJump();

	// Handles jump start
	UFUNCTION()
	void OnStartRun();

	// Handles jump start
	UFUNCTION()
	void OnStopRun();

	// Running animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// Idle animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// Idle animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

	// update animation depending on state
	void UpdateAnimation();

};
