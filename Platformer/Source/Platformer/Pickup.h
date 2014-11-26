// Fill out your copyright notice in the Description page of Project Settings.

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UENUM(BlueprintType)
namespace EBoostType
{
    enum Type
    {
        HP          UMETA(DisplayName = "HP boost"),
        Stamina		UMETA(DisplayName = "Stamina boost"),
        Attack		UMETA(DisplayName = "Attack boost"),
        Jump        UMETA(DisplayName = "Jump boost"),
    };
}

UCLASS()
class PLATFORMER_API APickup : public AActor
{
	GENERATED_UCLASS_BODY()
    
    // the value and type of the boost
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pickup)
	uint32 BoostValue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
    TEnumAsByte<EBoostType::Type> BoostType;
    
    // the possible collision area of the pickup
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Collision)
    TSubobjectPtr<UBoxComponent> BaseCollisionComponent;
    
	// the pickup sprite
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TSubobjectPtr<class UStaticMeshComponent> Sprite;
    
    // handles being picked up by the Hero
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
