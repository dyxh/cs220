// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "PlatformerHUD.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMER_API APlatformerHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	// stores HUD font
	UPROPERTY()
	UFont* HUDFont;
	
	// primary draw call for the HUD
	virtual void DrawHUD() override;
};
