// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "Pickup.h"
#include "PaperPlatformerCharacter.h"

//on collision, check whether the object is the character, if yes, add boost amount to stats
APickup::APickup(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
    
    boost = (rand() % 1000) / 10;//between 0 and 100, in 10s
    //int index = (rand() % 3);//0, 1, or 2
    BoostType = EnumType::HP;//either HP, Stamina, or Attack
    
    
    //create root SphereComponent to handle the pickup's collision
    BaseCollisionComponent = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("box"));
    //RootComponent = BaseCollisionComponent;
    
    BaseCollisionComponent->SetBoxExtent(FVector(40.0f, 0.0f, 60.0f));
    BaseCollisionComponent->AttachTo(RootComponent);
    //extends 40 units from character
    //RootComponent->SetBoxExtent(FVector(40.0f, 0.0f, 60.0f));
    
    //create static mesh component
    Sprite = PCIP.CreateDefaultSubobject<UPaperFlipbookComponent>(this, TEXT("PickupMesh"));
    
    //turn physics on
    PickupMesh->SetSimulatePhysics(true);
    
    //Attach the StaticMeshComponent to the root component
    PickupMesh->AttachTo(RootComponent);
    
    //passes in a a function pointer that binds to a collision, and acts accordingly
    BaseCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
    
}


//update stats and destroy item
void APickup::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult){
    
    //array of all actors that have
    TArray<AActor*> OverlapActors;
    BaseCollisionComponent->GetOverlappingActors(OverlapActors);
    
    for( auto overlapped : OverlapActors){
        
        APaperPlatformerCharacter* const Hero = Cast<APaperPlatformerCharacter>(overlapped);
        if(Hero){
        switch(BoostType){
            case(EnumType::HP):
                Hero->Health += boost;
                //                if(Hero->Health >= Hero->MaxHealth){
                //                    Hero->Health = MaxHealth;
                //                }
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HP boost!"));
                break;
            case(EnumType::Stamina):
                Hero->Stamina += boost;
                //                if(Hero->Stamina >= Hero->MaxStamina){
                //                    Hero->Stamina = MaxStamina;
                //                }
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Stamina boost!"));
                break;
            case(EnumType::Attack):
                Hero->AttackPower += boost;
                //                if(Hero->AttackPower >= Hero->MaxAttackPower){
                //                    Hero->AttackPower = MaxAttackPower;
                //                }
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Attack boost!"));
                break;
        }
            Destroy();
            //default action to remove mesh

        }
    }
    
    Destroy();
    
    
}

