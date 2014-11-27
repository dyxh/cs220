// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once

//#include "GameFramework/Actor.h"
//#include "Pickup.generated.h"
#include "PaperPlatformerCharacter.h"
#ifndef __Platformer_copy__APickup__
#define __Platformer_copy__APickup__
/**
 * 
 */
//UENUM(BlueprintType)
namespace EBoostType
{
    enum Type
    {
        HP,
        Stamina,
        Attack,
        Jump,
        Victory,
    };
}


//UCLASS()
class APickup
{
    APickup();
    ~APickup();
    //amount of hp/stamina/attack boosted
	
    float BoostValue;
    EBoostType::Type BoostType;
    //UFUNCTION()
    void ReceiveHit(class APaperPlatformerCharacter* Hero);
};

#endif
