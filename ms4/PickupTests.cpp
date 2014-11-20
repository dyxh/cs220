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
    
    character.Stamina = 50;
    MaxStamina = character.MaxStamina;
    //starting stamina
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    //first arg: true if overlapping, false if not
    EXPECT_EQ(character.Stamina, 60);
    
    //checks case where character is already at max stamina
    AStaminaPickup a;
    a.boost = 10;    
    character.Stamina = MaxStamina;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    EXPECT_EQ(character.Stamina, MaxStamina);
    
    //checks case where the potion heals stamina for an amount greater than 
    //the difference between max stamina and current stamina
    AStaminaPickup a;
    a.boost = 10;    
    character.Stamina = MaxStamina - 5;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    EXPECT_EQ(character.Stamina, MaxStamina);
}

TEST_F(TEST_PICKUP, TEST_HP){
    
    AHPPickup a;
    a.boost = 10;
    
    character.Health = 10;
    MaxHealth = character.MaxHealth;
    
    //checks the normal case
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    //first arg: true if overlapping, false if not
    EXPECT_EQ(character.Health, 20);
    
    //checks case where character is already at max health
    AHPPickup a;
    a.boost = 10;
    character.Health = MaxHealth;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    EXPECT_EQ(character.Health, MaxHealth);
    
    //checks case where the potion heals health for an amount greater than 
    //the difference between max health and current health
    AHPPickup a;
    a.boost = 10;
    character.Health = MaxHealth - 5;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    EXPECT_EQ(character.Health, MaxHealth);
    
}


TEST_F(TEST_PICKUP, TEST_ATTACK){
    
    AAttackPickup a;
    a.boost = 10;
    
    character.AttackPower = 100;
    
    EXPECT_TRUE(a.ApplyPickup(true, &character));
    //first arg: true if overlapping, false if not
    EXPECT_EQ(character.AttackPower, 110);
    
}
