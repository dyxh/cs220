// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PlatformerHUD.h"
#include "PaperPlatformerCharacter.h"
#include "PlatformerGameMode.h"


APlatformerHUD::APlatformerHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	// use the RobotoDistanceField font
    // static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/Content/EngineFonts/RobotoDistanceField"));
    // HUDFont = HUDFontOb.Object;
}

void APlatformerHUD::DrawHUD()
{
	// get screen dimensions
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	Super::DrawHUD();

	// get the character and print its Health and Stamina
	APaperPlatformerCharacter* MyCharacter = Cast<APaperPlatformerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FString LevelString = FString::Printf(TEXT("LVL: %5d"), MyCharacter->Level);
	FString ExpString = FString::Printf(TEXT("EXP: %5d / %5d"), MyCharacter->Experience, MyCharacter->MaxExperience);
	FString HealthString = FString::Printf(TEXT("HP: %5d / %5d"), MyCharacter->Health, MyCharacter->MaxHealth);
	FString StaminaString = FString::Printf(TEXT("STA: %5.1f / %5.1f"), MyCharacter->Stamina, MyCharacter->MaxStamina);
    FString AttackBuffString = FString::Printf(TEXT("BUF: %5.2f // ATK: %5.2f"), MyCharacter->AttackBuffDuration, MyCharacter->AttackPower);
    
    // drawing the stuff
	DrawText(LevelString, FColor::White, Canvas->SizeX - 250, 25, NULL);
	DrawText(ExpString, FColor::White, Canvas->SizeX - 250, 40, NULL);
    DrawText(AttackBuffString, FColor::White, Canvas->SizeX - 250, 55, NULL);
	DrawText(HealthString, FColor::Red, Canvas->SizeX - 250, 70, NULL);
	DrawText(StaminaString, FColor::Blue, Canvas->SizeX - 250, 85, NULL);
}
