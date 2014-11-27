// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

//#include "Platformer.h"
#include "Pickup.h"
//#include "APaperPlatformerCharacter.h"

//on collision, check whether the object is the character, if yes, add boost amount to stats
APickup::APickup()
{
    
	// determines the strength of the boost, random value 1, 2, or 3
	BoostValue = (rand() % 3) + 1;

	// BoostType defaults to HP, but can be set to Stamina or Attack in the Editor
	BoostType = EBoostType::HP;
    
}

APickup::~APickup()
{
    
}


//update stats and destroy item
void APickup::ReceiveHit(class APaperPlatformerCharacter* Hero)
{
	/* handles being picked up by the hero */
	Hero->OnItemPickup(BoostValue, BoostType);
	this->~APickup();
}

