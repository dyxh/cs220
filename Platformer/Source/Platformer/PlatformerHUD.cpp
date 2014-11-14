// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PlatformerHUD.h"
#include "PaperPlatformerCharacter.h"
#include "PlatformerGameMode.h"


APlatformerHUD::APlatformerHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

	// use the RobotoDistanceField font
//	static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/Content/EngineFonts/RobotoDistanceField"));
//	HUDFont = HUDFontOb.Object;
}

void APlatformerHUD::DrawHUD()
{
	// Get screen Dimensions
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	Super::DrawHUD();

	// get the character and print its Health and Stamina
	APaperPlatformerCharacter* MyCharacter = Cast<APaperPlatformerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FString HealthString = FString::Printf(TEXT("Health : %10.1f / %10.1f"), MyCharacter->Health, MyCharacter->MaxHealth);
	FString StaminaString = FString::Printf(TEXT("Stamina : %10.1f / %10.1f"), MyCharacter->Stamina, MyCharacter->MaxStamina);
	DrawText(HealthString, FColor::Red, 25, 25, NULL);
	DrawText(StaminaString, FColor::Black, 25, 45, NULL);
}
