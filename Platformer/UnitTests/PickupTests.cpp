#include <stdio.h>
#include <gtest/gtest.h>
#include "Pickup.h"
#include "StaminaPickup.h"
#include "HPPickup.h"
#Include "AttackPickup.h"
#include "APaperPlatformerCharacter.h"

class TEST_PICKUP: public ::testing::Test {
protected:
    APaperPlatformerCharacter character; // default values for all members

};


TEST_F(TEST_PICKUP, TEST_STAMINA){
    
    AStaminaPickup a;
    a.boost = 10;
    
    character.PlayerInput(ETestInput::Attack_Pressed);
    float initialStamina = character.Stamina;
    //stamina after attack
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    //first arg: true if overlapping, false if not
    EXPECT_EQ(character.Stamina, initialStamina + 10);
    
}

TEST_F(TEST_PICKUP, TEST_HP){
    
    AHPPickup a;
    a.boost = 10;
    
    character.Health = 0;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    //first arg: true if overlapping, false if not
    EXPECT_EQ(character.Health, a.boost);
}


TEST_F(TEST_PICKUP, TEST_HP){
    
    AAttackPickup a;
    a.boost = 10;
    
    character.AttackPower = 100;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    //first arg: true if overlapping, false if not
    EXPECT_EQ(character.AttackPower, 110);
    
}