// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PaperEnemy.h"
#include "PaperPlatformerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

APaperEnemy::APaperEnemy(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    // set initial health
    Health = MaxHealth = 1000.0f;

	ExperienceEnemy = 50;

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

void APaperEnemy::ReceiveHit(class UPrimitiveComponent *MyComp, AActor *Other,
                             class UPrimitiveComponent *OtherComp,
                             bool bSelfMoved, FVector HitLocation,
                             FVector HitNormal, FVector NormalImpulse,
                             const FHitResult &Hit)
{
    /* HANDLES A HERO BUMPING INTO IT */
    Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved,
                      HitLocation, HitNormal, NormalImpulse, Hit);
    
    APaperPlatformerCharacter *Hero = Cast<APaperPlatformerCharacter>(Other);
    
    if (Hero)
    {
        if (Hero->BattleState != EBattleState::Shield)
        {
            Hero->OnEnemyCollide(1);
            Hero->CharacterMovement->Velocity += HitNormal *
                                                 FVector(450.0f, 0.0f, 100.0f);
        }
    }
}