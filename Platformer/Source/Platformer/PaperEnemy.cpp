// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PaperEnemy.h"


APaperEnemy::APaperEnemy(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    // set initial health
    Health = MaxHealth = 1000.0f;
}

void APaperEnemy::ReceiveDamage(float val)
{
    // can be made more complicated than this
    // for example, we could reduce the damage by some some defense rating
    Health -= val;
    
    if (Health <= 0)
    {
        GEngine->AddOnScreenDebugMessage(2, 2.5f, FColor::Red, TEXT("Enemy defeated!!"));
        Destroy();
    }
}


