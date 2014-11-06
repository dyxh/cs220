#include "gtest/gtest.h"

class TEST_BATTLESTATE: public ::testing::Test {
protected:
    AOCharacter character = AOCharacter(); // default values for all members
}

TEST_F(TEST_BATTLESTATE, TEST_IDLE_STATE) 
{
    // Default character state is Idle
    BSIdle idleState = BSIdle();
    EXPECT_TRUE(character.get_BattleState() == idleState);
    
    //Check that stamina can't be regenerated past the max
    character.setStamina(character.maxStamina());
    character.update();
    EXPECT_TRUE(character.getStamina == character.maxStamina);
    
    //Check that stamina is being regenerated during idle state
    character.setStamina(0);
    character.update();
    EXPECT_TRUE(character.getStamina() > 0);
}

TEST_F(TEST_BATTLESTATE, TEST_ATTACK_STATE) 
{
    
    BSAttack attackState = BSAttack();
    BSIdle idleState = BSIdle();
    character.handleInput(&character, "press_attack");
    EXPECT_TRUE(character.get_BattleState() == attackState);
    
    int beforeAttackStamina = character.getStamina();
    // We expect attacks to last for 3 frames, transitioning back to idlestate after the third update
    // calls to AOCharacter::update() call the update function on all appropriate components of the character
    // Check that attacks successfully reduce stamina
    EXPECT_TRUE(character.getStamina() == beforeAttackStamina - 5);
    character.update();
    character.update();
 
    // Should still be in battle state after two updates
    EXPECT_TRUE(character.get_BattleState() == attackState));
    character.update();
    EXPECT_TRUE(character.get_BattleState() == idleState);
    
    character.handleInput(&character, "press_attack");
    character.update();
    character.update();
    character.handleInput(&character, "press_attack");
    EXPECT_TRUE(character.get_BattleState() == attackState);
    character.update();
    // Should still return to idle state even though attack was pressed again
    // during the attack state
    EXPECT_TRUE(character.get_BattleState() == idleState);
    
    // Can't attack without stamina
    character.setStamina = 0;
    character.handleInput(&character, "press_attack");
    EXPECT_TRUE(character.get_BattleState() == idleState);
    
}

TEST_F(TEST_BATTLESTATE, TEST_DAMAGED_STATE) 
{
    int maxHealth = character.maxHealth;
    BSDamaged damagedState = BSDamaged();
    EXPECT_TRUE(character.currentHealth == maxHealth);
    
    // tests if character takes damage
    character.takeDamage(10);
    EXPECT_TRUE(character.get_BattleState() == damagedState);
    EXPECT_TRUE(character.currentHealth == maxHealth - 10);
    
    // tests if character dies
    character.takeDamage(maxHealth - 10);
    EXPECT_TRUE(character.currentHealth == 0);
}

TEST_F(TEST_BATTLESTATE, TEST_SHIELD_STATE)
{

    BSShield shieldState = BSShield();
    BSIdle idleState = BSIdle();
    int preStam = character.getMaxStamina();
    character.handleInput(&character, "press_shield");
    
    // change to shield state
    EXPECT_TRUE(character.get_BattleState() == shieldState);
    character.update();
    
    // shielding should lower stamina
    EXPECT_TRUE(preStam > character.getCurrStamina());
    character.update();
    character.update();
    character.handleInput(&character, "release_shield");
    
    // releasing shield should return to idle state
    EXPECT_TRUE(character.get_BattleState() == idleState);
    
    // release shield when stamina is 0 (runs out)
    character.handleInput(&character, "press_shield");
    character.setStamina(0);
    character.update();
    EXPECT_TRUE(character.get_BattleState() == idleState);
    
    // cannot shield without stamina
    character.setStamina(0);
    character.handleInput(&character, "press_shield");
    EXPECT_TRUE(character.get_BattleState() == idleState);
}

