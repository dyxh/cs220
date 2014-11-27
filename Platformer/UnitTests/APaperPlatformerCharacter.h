//
//  APaperPlatformerCharacter.h
//  Platformer copy
//
//  Created by John Salem on 11/13/14.
//  Copyright (c) 2014 EpicGames. All rights reserved.
//


#ifndef __Platformer__APaperPlatformerCharacter__
#define __Platformer__APaperPlatformerCharacter__
#include "Pickup.h"


// PaperPlatformerCharacter.h
/**
 * Enumeration of movement states the character can be in
 */
namespace EMoveState
{
    enum State
    {
        Idle,
        Run,
        Death,
    };
}

/**
 * Enumeration of battle states the character can be in
 */
namespace EBattleState
{
    enum State
    {
        Idle,
        Shield,
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
    
    // Move State
    EMoveState::State MoveState;
    
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
    
    // Attack Power Increase per Level Increase
    float AttackPowerIncrease;
    
    //Base Attack power before modifiers
    float BaseAttackPower;
    
    // Length of buff
    float AttackBuffDuration;
    
    // Length of attack
    float AttackDuration;
    
    // Stamina cost of run (per tick)
    float StaminaRunCost;
    
    // Stamina cost of shield (per tick)
    float StaminaShieldCost;
    
    // Stamina cost of attack
    float StaminaAttackCost;
    
    // Tick
    void Tick();
    
    // setup user input
    void PlayerInput(ETestInput::Input input);
    
    // Maximum jumps possible
    int MaxJumps;
    
    // Jumps used
    int CurrentJumps;
    
    // Current experience of player
    int Experience;
    
    // Maximum experience for current level
    int MaxExperience;
    
    // Maximum experience increase per Level Increase
    int MaxExperienceIncrease;
    
    // Current level of player
    int Level;
    
    
protected:
    // Handles attack start
    void OnStartAttack();
    
    // Handles attack start
    void OnStopAttack();
    
    // Handles shield start
    void OnStartShield();
    
    // Handles shield start
    void OnStopShield();
    
public:
    // void OnEnemyCollide(float val);
    void OnItemPickup(float BoostValue, EBoostType::Type BoostType);
};

#endif