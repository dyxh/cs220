// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
namespace EnumType
{
    enum bType
    {
        HP          UMETA(DisplayName = "HP boost"),
        Stamina		UMETA(DisplayName = "Stamina boost"),
        Attack		UMETA(DisplayName = "Attack boost"),
    };
}


UCLASS()
class PLATFORMER_API APickup : public AActor
{
	GENERATED_UCLASS_BODY()
    
    //amount of hp/stamina/attack boosted
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	float boost;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<EnumType::bType> BoostType;
    
    //damage collision box
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
    TSubobjectPtr<UBoxComponent> BaseCollisionComponent;
    
	// The sprite of the pickup
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TSubobjectPtr<class UStaticMeshComponent> Sprite;
    
    // Handles being picked up by a character
    virtual void ReceiveHit(
        class UPrimitiveComponent *MyComp,
        AActor *Other,
        class UPrimitiveComponent *OtherComp,
        bool bSelfMoved,
        FVector HitLocation,
        FVector HitNormal,
        FVector NormalImpulse,
        const FHitResult &Hit
    ) override;
};
