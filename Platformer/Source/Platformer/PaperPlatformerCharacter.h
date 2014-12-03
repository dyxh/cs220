
#pragma once

#include "PaperCharacter.h"
#include "Pickup.h"
#include "PaperPlatformerCharacter.generated.h"

UENUM(BlueprintType)
namespace EMoveState
{
	enum State
	{
		Idle		UMETA(DisplayName = "Idle"),
		Run			UMETA(DisplayName = "Run"),
        Death       UMETA(DisplayName = "Death"),
	};
}

UENUM(BlueprintType)
namespace EBattleState
{
	enum State
	{
		Idle		UMETA(DisplayName = "Idle"),
		Shield		UMETA(DisplayName = "Shield"),
		Flinch		UMETA(DisplayName = "Flinch"),
		Attack		UMETA(DisplayName = "Attack"),
	};
}


UCLASS()
class PLATFORMER_API APaperPlatformerCharacter : public APaperCharacter
{
	GENERATED_UCLASS_BODY()

	// 2D camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class UCameraComponent> SideViewCameraComponent;

	// boom connecting camera to player position
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;
    
    // hitbox defining the range the character can inflict damage
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage)
    TSubobjectPtr<class UBoxComponent> Hitbox;
	
	// current movement state of the Hero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<EMoveState::State> MoveState;

	// current battle state of the Hero
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	TEnumAsByte<EBattleState::State> BattleState;

	// CURRENT health of the Hero
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	int32 Health;

	// MAXIMUM health of the Hero
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	int32 MaxHealth;

	// CURRENT stamina of the Hero
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float Stamina;

	// MAXIMUM stamina of the Hero
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float MaxStamina;

	// REGENERATE RATE for stamina of the Hero
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaminaRegen;
    
    // CURRENT attack power of the Hero
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
    float AttackPower;
    
    // BASE (without modifier) attack power of the Hero
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack)
    float BaseAttackPower;
    
    // DURRATION of an attack power buff from an AttackPickup
    UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Attack)
    float AttackBuffDuration;
    
    UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Attack)
    float AttackDuration;
    
	// STAMINA COST of run (per tick)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaminaRunCost;

	// STAMINA COST of shield (per tick)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaminaShieldCost;

	// STAMINA COST of attack (per attack)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaminaAttackCost;

	// override Tick
	virtual void Tick(float DeltaSeconds) override;
    
    // Maximum jumps possible
    int32 MaxJumps;
    
    // Jumps used
    int32 CurrentJumps;

	// Current experience of player
	int32 Experience;

	// Maximum experience for current level
	int32 MaxExperience;

	// Maximum experience increase per Level Increase
	int32 MaxExperienceIncrease;

	// Current level of player
	int32 Level;

	// Attack Power Increase per Level Increase
	float AttackPowerIncrease;

protected:
	// sets up controller interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Handles left/right movement
	UFUNCTION()
	void MoveRight(float val);

	// Handles jump start
	UFUNCTION()
	void OnStartJump();
    
    // Adds multiple jump functionality
    UFUNCTION()
    virtual bool CanJumpInternal_Implementation() const override;
    
    // Resets CurrentJumps to 0
	virtual void OnLanded(const FHitResult& Hit) override;

	// Handles left/right movement
	UFUNCTION()
	void OnStopJump();

	// Handles jump start
	UFUNCTION()
	void OnStartRun();

	// Handles jump start
	UFUNCTION()
	void OnStopRun();

	// Handles attack start
	UFUNCTION()
	void OnStartAttack();

	// Handles attack start
	UFUNCTION()
	void OnStopAttack();
    

	// Handles shield start
	UFUNCTION()
	void OnStartShield();

	// Handles shield start
	UFUNCTION()
	void OnStopShield();

    // NON-ACTION ANIMATIONS
	// Running animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningAnimation;

	// Idle animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* IdleAnimation;

	// Idle animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* JumpAnimation;

    // ACTION ANIMATIONS
    // Idle shield animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook* IdleShieldAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UPaperFlipbook* RunningShieldAnimation;
    
    // Idle Attack animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
    class UPaperFlipbook* IdleAttackAnimation;
    
    //SwordSwoosh Sound Properties
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
    TSubobjectPtr<UAudioComponent> SwordSwoosh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
    class USoundWave* SwordSwooshSound;
    
    // Jump Sound Properties
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
    TSubobjectPtr<UAudioComponent> JumpSoundPointer;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
    class USoundWave* JumpSound;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
    TSubobjectPtr<UAudioComponent> MovingSoundPointer;
    
    UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Audio)
    class USoundWave* MovingSound;
    
	// update animation depending on state
	void UpdateAnimation();
    
 public:
	// Handles taking damage
	UFUNCTION()
	void OnEnemyCollide(float val);

	UFUNCTION()
	void OnItemPickup(float BoostValue, EBoostType::Type BoostType);

	// Handles saving and loading of character information
	UFUNCTION(BlueprintCallable, Category = Save)
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	bool LoadGame();
    
    UFUNCTION(BlueprintCallable, Category = NewGame)
    void ClearSaveData();

	bool isJumping;
};
