//
//  APaperPlatformerCharacter.cpp
//  Platformer copy
//
//  Created by John Salem on 11/13/14.
//  Copyright (c) 2014 EpicGames. All rights reserved.
//

#include "APaperPlatformerCharacter.h"
// PaperPlatformerCharacter.cpp
// Fill out your copyright notice in the Description page of Project Settings.

//#include "Platformer.h"
//#include "PaperPlatformerCharacter.h"
//#include "PaperFlipbookComponent.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "PaperEnemy.h"


APaperPlatformerCharacter::APaperPlatformerCharacter()
{
    // set baseline health/stamina
    Health = MaxHealth = 1000.0f;
    Stamina = MaxStamina = 1000.0f;
    
    HealthRegen = 0.0f;
    StaminaRegen = 0.010f;
    
    AttackPower = 500.0f;
    
	// Length of buff
	AttackBuffDuration = 0;

    //StaminaRunCost = 10.0f;
    StaminaShieldCost = 10.0f;
    StaminaAttackCost = 100.0f;
    
    //MaxJumps = 3;
    //CurrentJumps = 0;
    
    // set initial battle and movement states
    //MoveState = EMoveState::Idle;
    BattleState = EBattleState::Idle;
}


void APaperPlatformerCharacter::PlayerInput(ETestInput::Input input)
{
    // check input and calldelegate
    switch (input) {
        case ETestInput::Attack_Pressed:
            OnStartAttack();
            break;
        case ETestInput::Attack_Released:
            OnStopAttack();
            break;
        case ETestInput::Shield_Pressed:
            OnStartShield();
            break;
        case ETestInput::Shield_Released:
            OnStopShield();
            break;
        default:
            break;
    }
}

void APaperPlatformerCharacter::OnStartAttack()
{
    // going to use the following function to get all actors inside the damagebox
    // https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/UKismetSystemLibrary/BoxOverlapActors_NEW/index.html
    if (Stamina > StaminaAttackCost)
    {
        BattleState = EBattleState::Attack;
        Stamina -= StaminaAttackCost;
        
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ATTACK!"));    Stamina -= Stamina
        
        // TArray<AActor*> EnemiesInRange;
        
        // FVector BoxPos = DamageBox->GetComponentLocation();
        // FVector BoxExtent = DamageBox->GetScaledBoxExtent();
        // const TArray<TEnumAsByte<EObjectTypeQuery>> filter; //not used
        // const TArray<AActor*> ignore; //not used
        
        // UKismetSystemLibrary::BoxOverlapActors_NEW(GetWorld(), BoxPos, BoxExtent, filter, APaperEnemy::StaticClass(), ignore, EnemiesInRange);
        
        // iterate through overlapped enemies
        // for (auto &overlapped : EnemiesInRange)
        // {
        // We know that all returned actors are going to be PaperEnemies
        // Here is where we will do any battle calculations, e.g. add sword atkpwr etc
        // Cast<APaperEnemy>(overlapped)->ReceiveDamage(AttackPower);
        // } AttackCost;
    }
}

void APaperPlatformerCharacter::OnStopAttack()
{
    BattleState = EBattleState::Idle;
}

void APaperPlatformerCharacter::OnStartShield()
{
    if (Stamina >= StaminaShieldCost)
    {
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("SHIELD!"));
        BattleState = EBattleState::Shield;
    }
}

void APaperPlatformerCharacter::OnStopShield()
{
    BattleState = EBattleState::Idle;
}

void APaperPlatformerCharacter::Tick()
{
    // if trying to run and actually moving
    //    if ((MoveState == EMoveState::Run) && (GetVelocity().Size() > 0.0f))
    //    {
    //        if (Stamina >= StaminaRunCost)
    //        {
    //           Stamina -= StaminaRunCost;
    //        }
    //        else
    //        {
    //            OnStopRun();
    //        }
    //    }
    //    else // if not moving
    //    {
    //        OnStopRun();
    //    }
    //
    if (/**(MoveState == EMoveState::Idle) &&*/ (Stamina < MaxStamina) && (BattleState == EBattleState::Idle)) // stamina regen
    {
        Stamina += MaxStamina * StaminaRegen;
        Stamina = (Stamina > MaxStamina) ? MaxStamina : Stamina;
    }
    
    if (BattleState == EBattleState::Shield)
    {
        if (Stamina >= StaminaShieldCost)
        {
            Stamina -= StaminaShieldCost;
        }
        else
        {
            BattleState = EBattleState::Idle;
        }
    }
}

void APaperPlatformerCharacter::OnItemPickup(float BoostValue, EBoostType::Type BoostType)
{
	switch (BoostType)
	{
	case (EBoostType::HP) :
		Health += BoostValue;
		if (Health >= MaxHealth){
			Health = MaxHealth;
		}
		break;
	case (EBoostType::Stamina) :
		Stamina += BoostValue * 100;
		if (Stamina >= MaxStamina)
		{
			Stamina = MaxStamina;
		}
		break;
	case (EBoostType::Attack) :
		AttackBuffDuration += 20;
		break;
	case (EBoostType::Jump) :
		if (MaxJumps < 3)
		{
			MaxJumps += 1;
		}
		break;
	case (EBoostType::Victory) :
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You've won the game! Open the menu to play again :)"));
	default:
		break;
	}
}