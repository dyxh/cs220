// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PlatformerHUD.h"
#include "PaperPlatformerCharacter.h"
#include "PlatformerGameMode.h"


APlatformerHUD::APlatformerHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}

void APlatformerHUD::DrawHUD()
{
	// get screen dimensions
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

    // draw the HUD
	Super::DrawHUD();

    // retrieves the Hero
    APaperPlatformerCharacter* Hero = Cast<APaperPlatformerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

    // creates Hero's level status string
    FString LevelString = FString::Printf(TEXT("LVL : %d"),
                                          Hero->Level);
    
    // creates Hero's experience status string
    FString ExpString = FString::Printf(TEXT("EXP : %d / %d"),
                                        Hero->Experience,
                                        Hero->MaxExperience);
    
    // creates Hero's stamina status string
    FString StaminaString = FString::Printf(TEXT("STA : %.0f / %.0f"),
                                            Hero->Stamina,
                                            Hero->MaxStamina);
    
    // creates Hero's health status string
    FString HealthString = FString::Printf(TEXT("HP : "));
    for (int32 i = 0; i < Hero->Health; ++i)
    {
        HealthString += FString::Printf(TEXT("<3 "));
    }
    
    // create's Hero's health status string
    FString AttackBuffString = FString::Printf(TEXT("ATK : %.0f > BUFF : %.1f"),
                                               Hero->AttackPower,
                                               Hero->AttackBuffDuration);
    
    // draws the status strings
	DrawText(LevelString, FColor::White, Canvas->SizeX - 270, 25, NULL);
	DrawText(ExpString, FColor::White, Canvas->SizeX - 270, 40, NULL);
	DrawText(HealthString, FColor::Red, Canvas->SizeX - 270, 55, NULL);
	DrawText(StaminaString, FColor::Blue, Canvas->SizeX - 270, 70, NULL);
    DrawText(AttackBuffString, FColor::White, Canvas->SizeX - 270, 85, NULL);
}
