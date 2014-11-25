// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PaperEnemy.h"
#include "PaperPlatformerCharacter.h"


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
        Destroy();
    }
}

void APaperEnemy::ReceiveHit(class UPrimitiveComponent *MyComp, AActor *Other, class UPrimitiveComponent *OtherComp,
                bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit)
{
    Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    APaperPlatformerCharacter *Hero = Cast<APaperPlatformerCharacter>(Other);
    
    if (Hero)
    {
        if (Hero->BattleState != EBattleState::Shield)
        {
            Hero->OnEnemyCollide(20.0f);
            Hero->CharacterMovement->Velocity += HitNormal * FVector(1000.0f, 0.0f, 250.0f);
        }
    }
    
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Enemy collision detected."));
}