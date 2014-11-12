// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PlatformerGameMode.h"
#include "Engine.h"
#include "PaperPlatformerCharacter.h"
#include "PlatformerHUD.h"


APlatformerGameMode::APlatformerGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	DefaultPawnClass = APaperPlatformerCharacter::StaticClass();
	HUDClass = APlatformerHUD::StaticClass();
}

void APlatformerGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLD"));
	}
}