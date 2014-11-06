/*
 * test_inventory.cpp
 *
 *  Created on: Nov 5, 2014
 *
 */

#include "gtest/gtest.h"
#include "Inventory.h"
#include "Item.h"
#include "IWeapon.h"
#include "IPotion.h"
#include "IBuff.h"

class test_inventory: public ::testing::Test {
protected:
	Inventory a();
	IWeapon w1(1,"sword","basic sword",10);
	IPotion p1(4,"p1","heals 10 hp",10);
	IBuff b1(7,"red","Increases atk by 10%",1.1);
};

TEST_F(test_inventory, add_invItem){

	//adding one of each of two different items
	EXPECT_TRUE(a.add_invItem(w1));
	EXPECT_TRUE(a.add_invItem(p1));

	//tests size of the inventory and quantity of each item
	EXPECT_EQ(a.inv.size(),2);
	EXPECT_EQ(a.inv.find(w1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(p1.get_id())->second.second, 1);

	//tests size of inventory after adding another different item
	//makes sure that the quantity of the other items do not change
	EXPECT_TRUE(a.add_invItem(b1));
	EXPECT_EQ(a.inv.size(), 3);
	EXPECT_EQ(a.inv.find(w1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(p1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(b1.get_id())->second.second, 1);

	//tests size of inventory does not change after adding one more
	//of the same item; quantity of one item increases by one
	EXPECT_TRUE(a.add_invItem(w1));
	EXPECT_EQ(a.inv.size(), 3);
	EXPECT_EQ(a.inv.find(w1.get_id())->second.second, 2);
	EXPECT_EQ(a.inv.find(p1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(b1.get_id())->second.second, 1);
}

TEST_F(test_inventory, remove_invItem){
	//adds 2 of the weapon and 1 of a potion and a buff
	EXPECT_TRUE(a.add_invItem(w1));
	EXPECT_TRUE(a.add_invItem(p1));
	EXPECT_TRUE(a.add_invItem(b1));
	EXPECT_TRUE(a.add_invItem(w1));

	//checks if there are two of the first weapon
	EXPECT_EQ(a.inv.size(),3);
	EXPECT_EQ(a.inv.find(w1.get_id())->second.second, 2);
	EXPECT_EQ(a.inv.find(p1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(b1.get_id())->second.second, 1);

	//checks if removing one of an item with quantity
	//greater than 1 lowers quantity by 1 but does not
	//remove the item from the map nor change quantity 
	//of other items
	EXPECT_TRUE(a.remove_invItem(w1));
	EXPECT_EQ(a.inv.size(), 3);
	EXPECT_EQ(a.inv.find(w1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(p1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(b1.get_id())->second.second, 1);

	//checks if removing one of an item with quantity 1
	//would remove the item from the map
	EXPECT_TRUE(a.remove_invItem(w1));
	EXPECT_EQ(a.inv.size(),2);
	EXPECT_TRUE(a.inv.find(w1.get_id()) == a.inv.end());
	EXPECT_EQ(a.inv.find(p1.get_id())->second.second, 1);
	EXPECT_EQ(a.inv.find(b1.get_id())->second.second, 1);
}
