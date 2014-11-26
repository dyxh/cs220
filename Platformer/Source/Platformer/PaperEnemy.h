
#pragma once

#include "PaperCharacter.h"
#include "PaperEnemy.generated.h"

UCLASS()
class PLATFORMER_API APaperEnemy : public APaperCharacter
{
	GENERATED_UCLASS_BODY()

	// current health
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
    int32 Health;
    
    // Max health of enemy
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
    int32 MaxHealth;
	
	// Experience that enemy will give player
	int32 ExperienceValue;

    // Calculates health loss from given damage
    UFUNCTION()
    void ReceiveDamage(float val);
    
    // Handles damaging the player
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