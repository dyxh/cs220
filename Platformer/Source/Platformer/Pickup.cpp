// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "Pickup.h"
#include "PaperPlatformerCharacter.h"

APickup::APickup(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
    /* constructor */
    
    // determines the strength of the boost, random value 1, 2, or 3
    BoostValue = (rand() % 3) + 1;
    
    // BoostType defaults to HP, but can be set to Stamina or Attack in the Editor
    BoostType = EBoostType::HP;
    
    // create root SphereComponent to handle the pickup's collision
    BaseCollisionComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("PickupBox"));
    BaseCollisionComponent->SetBoxExtent(FVector(5.0f, 0.0f, 5.0f));
    RootComponent = BaseCollisionComponent;

    // create static mesh component
    Sprite = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));
    Sprite->SetSimulatePhysics(true);
    Sprite->AttachTo(RootComponent);
}

void APickup::ReceiveHit(class UPrimitiveComponent *MyComp, AActor *Other,
                         class UPrimitiveComponent *OtherComp, bool bSelfMoved,
                         FVector HitLocation, FVector HitNormal,
                         FVector NormalImpulse, const FHitResult &Hit)
{
    /* handles being picked up by the hero */
    
    Super::ReceiveHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    APaperPlatformerCharacter *Hero = Cast<APaperPlatformerCharacter>(Other);
    
    if (Hero)
    {
        Hero->OnItemPickup(BoostValue, BoostType);
        Destroy();
    }
}


