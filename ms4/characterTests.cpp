#include <stdio.h>
#include <gtest/gtest.h>
#include "APaperPlatformerCharacter.h"

class TEST_CHARACTER: public ::testing::Test {
protected:
    APaperPlatformerCharacter character; // default values for all members
};

TEST_F(TEST_CHARACTER, TEST_SWIM_STATE)
{
    character.isInWater = true;
    EMoveState::State swimState = EMoveState::Swim;
    EXPECT_TRUE(character.MoveState == swimState);
    EXPECT_TRUE(gravity == 0.5);
    character.PlayerInput(ETestInput::Jump_Pressed);
    EXPECT_TRUE(character.CurrentJumps == 0);
    
}

TEST_F(TEST_CHARACTER, TEST_CLIMB_STATE)
{
    EMoveState::State climbState = EMoveState::Climb;
    EMoveState::state idleState = EMoveState::Idle

    character.MoveState = idleState;
    character.isNearClimbableObject = true;
    character.PlayerInputETestInput::Climb_Pressed);
    EXPECT_TRUE(character.MoveState == climbState);
    EXPECT_TRUE(gravity == 0);

    character.PlayerInputETestInput::Jump_Pressed);
    EXPECT_TRUE(character.MoveState == idleState);

    character.MoveState = climbState;
    character.isNearClimbableObject = false;
    EXPECT_TRUE(character.MoveState == idleState);
}

TEST_F(TEST_CHARACTER, TEST_LIFT_STATE)
{
    character.isNearMovableObject = true;
    
    EMoveState::State liftState = EMoveState::Lift;
    EMoveState::state idleState = EMoveState::Idle;
    
    float beforeLiftStamina = character.Stamina;
    
    character.PlayerInput(ETestInput::Lift_Pressed);
    character.Tick();
    
    EXPECT_TRUE(character.MoveState == liftState);
    EXPECT_TRUE(CharacterMovement->MaxWalkSpeed == 400.0f);
    EXPECT_TRUE(character.CurrentJumps == character.MaxJumps); // e.g. can't jump
    EXPECT_TRUE(character.Stamina < beforeLiftStamina);
    
    character.PlayerInput(ETestInput::Lift_Pressed);
    character.Tick();
    EXPECT_TRUE(character.MoveState == idleState);   
}

TEST_F(TEST_CHARACTER, TEST_DEATH_STATE)
{
    EMoveState::State deathState = EMoveState::Death;
    
    character.Health = 0.0f;
    EXPECT_TRUE(character.MoveState == deathState);
}

TEST_F(TEST_CHARACTER, TEST_LEVELING)
{
    int currentLevel = character.CurrentLevel;
    int maxXP = character.MaxXP;
    int currentXP = character.CurrentXP;
    int attackPower = character.AttackPower;
    float maxHealth = character.MaxHealth;
    
    character.LevelUp();
    EXPECT_TRUE(character.CurrentLevel == currentLevel + 1);
    EXPECT_TRUE(character.MaxXP > maxXP);
    EXPECT_TRUE(character.CurrentXP == 0);
    EXPECT_TRUE(character.AttackPower > attackPower);
    EXPECT_TRUE(character.MaxHealth > maxHealth);
}

TEST_F(TEST_CHARACTER, TEST_MURDER)
{
    int currentXP = character.CurrentXP;
    int killXP = 20;
    character.OnKill(killXP);
    EXPECT_TRUE(character.CurrentXP = currentXP + killXP);
}

TEST_F(TEST_CHARACTER, TEST_DOUBLEJUMP)
{
    character.MaxJumps = 2;
    character.CurrentJumps = 0;
    
    // test double jump
    character.PlayerInput(ETestInput::Jump_Pressed);    
    EXPECT_TRUE(character.CurrentJumps == 1);

    character.PlayerInput(ETestInput::Jump_Pressed);    
    EXPECT_TRUE(character.CurrentJumps == 2);

    character.PlayerInput(ETestInput::Jump_Pressed);    
    EXPECT_TRUE(character.CurrentJumps == 2);

    character.OnLand = true;
    EXPECT_TRUE(character.CurrentJumps == 0);
    
    // test single jump
    character.PlayerInput(ETestInput::Jump_Pressed);    
    EXPECT_TRUE(character.CurrentJumps == 1);

    character.OnLand = true;
    EXPECT_TRUE(character.CurrentJumps == 0);
}

TEST_F(TEST_CHARACTER, TEST_SAVEGAME)
{
    LoadGame();
    int level = CurrentGame.Level;
    int location = CurrentGame.Location;

    float maxHealth = character.MaxHealth;
    float attack = character.AttackPower;
    int maxJumps = character.MaxJumps;
    int currentXP = character.CurrentXP;
    int maxXP = character.MaxXP;
    int currentLevel = character.CurrentLevel;
    
    // apply changes
    GoToNextLevel();
    
    // get the state
    SaveGame();
    
    // compare the state
    EXPECT_TRUE(CurrentGame.Level == level + 1);
    EXPECT_TRUE(character.Health == character.MaxHealth);
    EXPECT_TRUE(character.Stamina == character.MaxStamina);
    EXPECT_TRUE(character.Attack == attack);
    EXPECT_TRUE(character.maxJumps == maxJumps);
    EXPECT_TRUE(character.currentXP = CurrentXP);
    EXPECT_TRUE(character.MaxXP == maxXP);
    EXPECT_TRUE(character.CurrentLevel == currentLevel);
}
