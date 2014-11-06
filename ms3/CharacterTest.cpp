#include "gtest/gtest.h"

class TEST_CHARACTER: public ::testing::Test{
protected:
    AOCharacter character = AOCharacter();
};

TEST_F(TEST_CHARACTER, TEST_HEALTH)
{
    // check initial health conditions
    EXPECT_TRUE(character.currentHealth == character.maxHealth);
    
    // check health reduction from damage
    int beforeDamageHealth = character.currentHealth;
    character.takeDamage(10);
    EXPECT_TRUE(character.currentHealth == beforeDamageHealth - 10);
    
    // check health can't be below zero
    character.takeDamage(character.currentHealth + 10);
    EXPECT_TRUE(character.currentHealth == 0);
}

TEST_F(TEST_CHARACTER, TEST_STAMINA)
{
    // check initial stamina
    EXPECT_TRUE(character.currentStamina == character.maxStamina);

    // check stamina can't be increased above max
    character.setStamina(character.maxStamina + 10);
    EXPECT_TRUE(character.currentStamina == character.maxStamina);
    
    // check stamina reduction;
    character.currentStamina() = character.maxStamina();
    character.useStamina(10);
    EXPECT_TRUE(character.currentStamina == character.maxStamina() - 10);

    // check stamina can't go below 0;
    character.setStamina(0);
    character.useStamina(10);
    EXPECT_TRUE(character.currentStamina == 0);
    
    // check that stamina regenerates correctly, assuming stamina increased by 10 every tick
    character.setStamina(character.maxStamina - 10);
    character.update(); // assuming regen rate is 10 stamina per frame
    EXPECT_TRUE(character.currentStamina == character.maxStamina);  
}

TEST_F(TEST_CHARACTER, TEST_ITEM)
{
    // check use health potion
    Item *item = ItemPool[0x0]; // where 0 is an item id for a health potion
    character.setHealth(90);
    item.use(&character);
    EXPECT_TRUE(character.currentHealth == 100); // assumes a potion restores 10 health points
    
    // check use stamina potion
    Item *item = ItemPool[0x1];// where 1 is an item id for a stamina potion
    character.setStamina(90);
    item.use(&character);
    EXPECT_TRUE(character.currentStamina == 100); // assumes a stamina potion restores 10 stamina points
    

    // item equip is just getting and setting member attributes
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    