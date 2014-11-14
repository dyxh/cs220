//
//  ChracterTests.cpp
//  Platformer copy
//
//  Created by John Salem on 11/13/14.
//  Copyright (c) 2014 EpicGames. All rights reserved.
//

#include <stdio.h>
#include <gtest/gtest.h>
#include "APaperPlatformerCharacter.h"

class TEST_CHARACTER: public ::testing::Test {
protected:
    APaperPlatformerCharacter character; // default values for all members
};

TEST_F(TEST_CHARACTER, TEST_IDLE_STATE)
{
    character = APaperPlatformerCharacter();
    
    // Default character state is Idle
    EBattleState::State idleState = EBattleState::Idle;
    EXPECT_TRUE(character.BattleState == idleState);
    
    //Check that stamina can't be regenerated past the max
    character.Tick();
    EXPECT_TRUE(character.Stamina == character.MaxStamina);
    
    //Check that stamina is being regenerated during idle state
    character.Stamina = 0;
    character.Tick();
    EXPECT_TRUE(character.Stamina > 0);
}

TEST_F(TEST_CHARACTER, TEST_ATTACK_STATE)
{
    character = APaperPlatformerCharacter();
    
    EBattleState::State attackState = EBattleState::Attack;
    EBattleState::State idleState = EBattleState::Idle;
    
    float beforeAttackStamina = character.Stamina;
    character.PlayerInput(ETestInput::Attack_Pressed);
    EXPECT_TRUE(character.BattleState == attackState);
    
    // Attacks cost 200 stamina
    EXPECT_TRUE(character.Stamina == beforeAttackStamina - 100);
    character.Tick();
    character.Tick();
    // Should still be in battle state after two updates
    EXPECT_TRUE(character.BattleState == attackState);
    character.PlayerInput(ETestInput::Attack_Released);
    EXPECT_TRUE(character.BattleState == idleState);
    
    // Can't attack without stamina
    character.Stamina = 0;
    character.PlayerInput(ETestInput::Attack_Pressed);
    EXPECT_TRUE(character.BattleState == idleState);
    
}

TEST_F(TEST_CHARACTER, TEST_SHIELD_STATE)
{
    
    EBattleState::State shieldState = EBattleState::Shield;
    EBattleState::State idleState = EBattleState::Idle;
    int preStam = character.MaxStamina;
    character.PlayerInput(ETestInput::Shield_Pressed);
    
    // change to shield state
    EXPECT_TRUE(character.BattleState == shieldState);
    character.Tick();
    
    // shielding should lower stamina
    EXPECT_TRUE(preStam > character.Stamina);
    character.Tick();
    character.Tick();
    character.PlayerInput(ETestInput::Shield_Released);
    
    // releasing shield should return to idle state
    EXPECT_TRUE(character.BattleState == idleState);
    
    // can't stay in shield state when stamina is 0 (runs out)
    character.PlayerInput(ETestInput::Shield_Pressed);
    character.Stamina = 0;
    character.Tick();
    EXPECT_TRUE(character.BattleState == idleState);
    
    // cannot shield without stamina
    character.Stamina = 0;
    character.PlayerInput(ETestInput::Shield_Pressed);
    EXPECT_TRUE(character.BattleState == idleState);
}

