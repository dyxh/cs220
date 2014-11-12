// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "PlatformerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API APlatformerGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()

	virtual void StartPlay() override;
	
};
