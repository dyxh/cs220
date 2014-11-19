#include <stdio.h>
#include <gtest/gtest.h>
#include "Pickup.h"
#include "APaperPlatformerCharacter.h"

class TEST_PICKUP: public ::testing::Test {
protected:
    APaperPlatformerCharacter character; // default values for all members

};


TEST_F(TEST_PICKUP, TEST_STAMINA){
    
    float beforeAttackStamina = character.Stamina;
    character.PlayerInput(ETestInput::Attack_Pressed);
    EXPECT_TRUE(character.BattleState == attackState);
    
    
    
    
}
