// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PaperEnemy.h"
#include "PaperPlatformerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

APaperEnemy::APaperEnemy(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    /* constructor */
    
    Health = MaxHealth = 15;
	ExperienceValue = 50;
    DamageValue = 1;
}

void APaperEnemy::ReceiveDamage(float val)
{
    /* handles being damaged by the Hero */
    
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
    /* handles damaging the Hero */
    
    Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved,
                      HitLocation, HitNormal, NormalImpulse, Hit);
    
    APaperPlatformerCharacter *Hero = Cast<APaperPlatformerCharacter>(Other);
    
    if (Hero)
    {
        if (Hero->BattleState != EBattleState::Shield)
        {
            Hero->OnEnemyCollide(DamageValue);
        }
        Hero->CharacterMovement->Velocity += HitNormal * FVector(450.0f, 0.0f, 100.0f);
    }
}