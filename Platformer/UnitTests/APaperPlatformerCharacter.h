//
//  APaperPlatformerCharacter.h
//  Platformer copy
//
//  Created by John Salem on 11/13/14.
//  Copyright (c) 2014 EpicGames. All rights reserved.
//

#ifndef __Platformer_copy__APaperPlatformerCharacter__
#define __Platformer_copy__APaperPlatformerCharacter__

#include <stdio.h>

#endif /* defined(__Platformer_copy__APaperPlatformerCharacter__) */

// PaperPlatformerCharacter.h
/**
 * Enumeration of movement states the character can be in
 */
//namespace EMoveState
//{
//    enum State
//    {
//        Idle,
//        Run
//    };
//}

/**
 * Enumeration of battle states the character can be in
 */
namespace EBattleState
{
    enum State
    {
        Idle,
        Shield,
        Flinch,
        Attack,
    };
}

namespace ETestInput
{
    enum Input
    {
        Attack_Pressed,
        Attack_Released,
        Shield_Pressed,
        Shield_Released,
    };
}

/**
 *
 */
class APaperPlatformerCharacter
{
public:
    APaperPlatformerCharacter();
    
    // Battle State
    EBattleState::State BattleState;
    
    // Current health of character
    float Health;
    
    // Max possible health of character
    float MaxHealth;
    
    // Current Stamina of Chracter
    float Stamina;
    
    // Max possible Stamina of Character
    float MaxStamina;
    
    // Stamina regeneration rate
    float StaminaRegen;
    
    // Health Regeneration rate
    float HealthRegen;
    
    // Health Regeneration rate
    float AttackPower;
    
    // Stamina cost of run (per tick)
    //float StaminaRunCost;
    
    // Stamina cost of shield (per tick)
    float StaminaShieldCost;
    
    // Stamina cost of attack
    float StaminaAttackCost;
    
    // Tick
    void Tick();
    
    // Maximum jumps possible
    //int MaxJumps;
    
    // Jumps used
    //int CurrentJumps;
    
    // setup user input
    void PlayerInput(ETestInput::Input input);
    
protected:
    // Handles attack start
    void OnStartAttack();
    
    // Handles attack start
    void OnStopAttack();
    
    // Handles shield start
    void OnStartShield();
    
    // Handles shield start
    void OnStopShield();
};