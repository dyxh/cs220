#include <stdio.h>
#include <gtest/gtest.h>
#include "Pickup.h"
#include "APaperPlatformerCharacter.h"

class TEST_PICKUP: public ::testing::Test {
protected:
    APaperPlatformerCharacter character; // default values for all members

};


TEST_F(TEST_PICKUP, TEST_STAMINA){
    
    char buffer[sizeof(APickup)];
    
    APickup* a = new(buffer)APickup;
	a->BoostType = EBoostType::Stamina;
    
	float BoostValue = a->BoostValue * 100; // this is the multiplier for stamina potions

	character.Stamina = 0;
	a->ReceiveHit(&character);
	EXPECT_TRUE(character.Stamina == BoostValue);


	character.Stamina = character.MaxStamina;
	APickup b;
	b.BoostType = EBoostType::Stamina;

	float BoostValueB = b.BoostValue * 100; // this is the multiplier for stamina potions
	b.ReceiveHit(&character);
	EXPECT_TRUE(character.Stamina == character.MaxStamina);
}

TEST_F(TEST_PICKUP, TEST_HP){
    
    APickup a;
    a.BoostType = EBoostType::HP;
	float BoostValue = a.BoostValue;
    character.Health = 0;
	a.ReceiveHit(&character);
	EXPECT_TRUE(character.Health == BoostValue);

	APickup b;
	b.BoostType = EBoostType::HP;
	float BoostValueB = b.BoostValue;
	character.Health = character.MaxHealth;
	b.ReceiveHit(&character);
	EXPECT_TRUE(character.Health == character.MaxHealth);
}


TEST_F(TEST_PICKUP, TEST_ATTACK){
    
	APickup a;
	a.BoostType = EBoostType::Attack;
	character.AttackBuffDuration = 0;
	a.ReceiveHit(&character);
	EXPECT_TRUE(character.AttackBuffDuration == 20); // attack buffs increase duration by 20
    
}