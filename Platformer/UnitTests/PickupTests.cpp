#include <stdio.h>
#include <gtest/gtest.h>
#include "Pickup.h"
#include "APaperPlatformerCharacter.h"

class TEST_PICKUP: public ::testing::Test {
protected:
    APaperPlatformerCharacter character; // default values for all members

};


TEST_F(TEST_PICKUP, TEST_STAMINA){
    
    APickup a;
	a.BoostType = EBoostType::Stamina;
    
	float BoostValue = a.BoostValue * 100; // this is the multiplier for stamina potions

	character.Stamina = 0;
    
	a.ReceiveHit(&character);
	EXPECT_TRUE(character.Stamina == BoostValue);
	EXPECT_TRUE(!a); // item destroyed itself



	character.Stamina = character.MaxStamina;
	APickup b;
	b.BoostType = EBoostType::Stamina;

	float BoostValueB = b.BoostValue * 100; // this is the multiplier for stamina potions
	b.ReceiveHit(&character);
	EXPECT_TRUE(character.Stamina == character.MaxStamina);
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