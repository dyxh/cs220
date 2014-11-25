// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SaveGame.h"
#include "CSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API UCSaveGame : public USaveGame
{
	GENERATED_UCLASS_BODY()

    //SaveSlotName and UserIndex used for default save slot
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString SaveSlotName;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxHealth;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxStam;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    float Attack;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 MaxXP;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 CurrentXP;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 MaxJumps;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 CurrentLevel;
	
};
