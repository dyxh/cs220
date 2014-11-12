// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperCharacter.h"
#include "PaperEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API APaperEnemy : public APaperCharacter
{
	GENERATED_UCLASS_BODY()

	// The Health of the enemy
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
    float Health;
    
    // Max health of enemy
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
    float MaxHealth;
	
    // Calculates health loss from given damage
    UFUNCTION()
    void ReceiveDamage(float val);
};
