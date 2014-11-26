// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "Pickup.h"
#include "PaperPlatformerCharacter.h"

APickup::APickup(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
    // random boost value 1, 2, or 3
    boost = (rand() % 3) + 1;
    
    // BoostType can be HP, Stamina or Attack
    BoostType = EnumType::HP;
    
    // create root SphereComponent to handle the pickup's collision
    BaseCollisionComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("PickupBox"));
    RootComponent = BaseCollisionComponent;
    BaseCollisionComponent->SetBoxExtent(FVector(50.0f, 0.0f, 50.0f));
    
    // create static mesh component
    Sprite = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));
    
    // turn physics on
    Sprite->SetSimulatePhysics(false);
    
    // attaches the StaticMeshComponent to the root component
    Sprite->AttachTo(RootComponent);
}

// handles being picked up by the Hero
void APickup::ReceiveHit(class UPrimitiveComponent *MyComp, AActor *Other,
                         class UPrimitiveComponent *OtherComp, bool bSelfMoved,
                         FVector HitLocation, FVector HitNormal,
                         FVector NormalImpulse, const FHitResult &Hit)
{
    Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    APaperPlatformerCharacter *Hero = Cast<APaperPlatformerCharacter>(Other);
    
    if (Hero)
    {
        Hero->OnItemPickup(boost, BoostType);
        Destroy();
    }
}


