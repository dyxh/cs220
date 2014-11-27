// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once

//#include "GameFramework/Actor.h"
//#include "Pickup.generated.h"
#ifndef __Platformer_copy__APickup__
#define __Platformer_copy__APickup__
/**
 * 
 */
//UENUM(BlueprintType)
namespace EnumType
{
    enum bType
    {
        HP,
        Stamina,
        Attack,
    };
}


//UCLASS()
class APickup
{
    APickup();
    //amount of hp/stamina/attack boosted
	
    float boost;
    
    //UFUNCTION()
    //void OnBeginOverlap();
};

#endif
