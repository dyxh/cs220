// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "OCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AOCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

    /** Side view camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    TSubobjectPtr<class UCameraComponent> SideViewCameraComponent;
    
    /** Camera boom positioning the camera beside the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
    TSubobjectPtr<class USpringArmComponent> CameraBoom;
    
protected:
    
    /** Called for side to side input */
    void MoveRight(float Val);
    
    /** Handle touch inputs. */
    void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
    
    /** Handle touch stop event. */
    void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);
    
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
    // End of APawn interface
    
};
