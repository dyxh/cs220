// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PaperPlatformerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PaperEnemy.h"

#include "CSaveGame.h"
#include "Kismet/GameplayStatics.h"

APaperPlatformerCharacter::APaperPlatformerCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    /* constructor */
    
	// enable tick
	PrimaryActorTick.bCanEverTick = true;
    
    // If saved file exists, load it
    if (!LoadGame())
    {
        // set base health
        Health = MaxHealth = 10;
    
        // set base stamina and stamina regen
        Stamina = MaxStamina = 1000.0f;

        // set base attack power
        AttackPower = BaseAttackPower = 10.0f;

        // set experience
        Experience = 0;
        MaxExperience = 100;
        Level = 1;
        
        //set number of possible jumps
        MaxJumps = 1;
    }
    else
    {
        //Initialize health, stamina, and attack power to their initial
        // values based upon the loaded file
        Health = MaxHealth;
        Stamina = MaxStamina;
        AttackPower = BaseAttackPower;
    }
    
    // Set attack power increase per level
    // duration of attack power increase buffs
    AttackPowerIncrease = 3.0f;
    AttackBuffDuration = 0.0f;
    AttackDuration = 0.0f;
    
    // set stamina costs and regeneration rate
	StaminaRunCost = 10.0f;
	StaminaShieldCost = 10.0f;
	StaminaAttackCost = 100.0f;
    StaminaRegen = 200.0f;

    // Sets the experience increase needed to level up after every level up
    MaxExperienceIncrease = 50;
    
    //initialize jump counter to 0
	CurrentJumps = 0;
    
	// set initial battle and movement states
	MoveState = EMoveState::Idle;
	BattleState = EBattleState::Idle;
    
	// get and setup the animations
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> JumpAnimationAsset;
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleShieldAnimationAsset;
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAttackAnimationAsset;
		ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> RunningShieldAnimationAsset;
        ConstructorHelpers::FObjectFinder<USoundWave> SwordSwooshAsset;
        ConstructorHelpers::FObjectFinder<USoundWave> JumpSoundAsset;
        ConstructorHelpers::FObjectFinder<USoundWave> MovingSoundAsset;
        
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/Flipbooks/PlayerRunning.PlayerRunning"))
			, IdleAnimationAsset(TEXT("/Game/Flipbooks/PlayerIdle.PlayerIdle"))
			, JumpAnimationAsset(TEXT("/Game/Flipbooks/PlayerJump.PlayerJump"))
            , IdleShieldAnimationAsset(TEXT("/Game/Flipbooks/PlayerIdleShield.PlayerIdleShield"))
            , IdleAttackAnimationAsset(TEXT("/Game/Flipbooks/PlayerIdleAttack.PlayerIdleAttack"))
			, RunningShieldAnimationAsset(TEXT("/Game/Flipbooks/PlayerRunningShield.PlayerRunningShield"))
            , SwordSwooshAsset(TEXT("/Game/Audio/sword-swoosh.sword-swoosh"))
            , JumpSoundAsset(TEXT("/Game/Audio/jump.jump"))
            , MovingSoundAsset(TEXT("/Game/Audio/footsteps.footsteps"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	JumpAnimation = ConstructorStatics.JumpAnimationAsset.Get();
    IdleAttackAnimation = ConstructorStatics.IdleAttackAnimationAsset.Get();
    IdleShieldAnimation = ConstructorStatics.IdleShieldAnimationAsset.Get();
	RunningShieldAnimation = ConstructorStatics.RunningShieldAnimationAsset.Get();
	Sprite->SetFlipbook(IdleAnimation);
    
    //set audio
    SwordSwooshSound = ConstructorStatics.SwordSwooshAsset.Object;
    
    SwordSwoosh = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("sword-swoosh"));
    if (SwordSwoosh)
    {
        SwordSwoosh->AttachParent = RootComponent;
        SwordSwoosh->bAutoActivate = false;
        SwordSwoosh->SetSound(SwordSwooshSound);
        SwordSwoosh->Activate();
    }
    
    JumpSound = ConstructorStatics.JumpSoundAsset.Object;
    
    JumpSoundPointer = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("jump sound"));
    if (JumpSoundPointer)
    {
        JumpSoundPointer->AttachParent = RootComponent;
        JumpSoundPointer->bAutoActivate = false;
        JumpSoundPointer->SetSound(JumpSound);
        JumpSoundPointer->Activate();
    }
    
    MovingSound = ConstructorStatics.MovingSoundAsset.Object;
    MovingSound->bLooping = true;
    
    MovingSoundPointer = PCIP.CreateDefaultSubobject<UAudioComponent>(this, TEXT("moving sound"));
    if( MovingSoundPointer)
    {
        MovingSoundPointer->AttachParent = RootComponent;
        MovingSoundPointer->bAutoActivate = false;
        MovingSoundPointer->SetSound(MovingSound);
        MovingSoundPointer->Activate();
    }
    

	// set capsule size
	CapsuleComponent->SetCapsuleHalfHeight(60.0f);
	CapsuleComponent->SetCapsuleRadius(40.0f);

	// ignore pitch and roll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Setup damage box
	Hitbox = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Hitbox"));
	Hitbox->SetBoxExtent(FVector(60.0f, 0.0f, 25.0f));
    Hitbox->AttachTo(RootComponent);
	Hitbox->RelativeLocation = FVector(70.0f, 0.0f, 10.0f);

	// attach camera boom to capsule
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(50.f, 0.f, 75.f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->RelativeRotation = FRotator(0.5, -90.f, 0.f);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.f;
	CameraBoom->bDoCollisionTest = false;

	// Create and attach orthographic cam to boom
	SideViewCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("2DViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
	SideViewCameraComponent->OrthoWidth = 1024.0f;
	SideViewCameraComponent->FieldOfView = 90.f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName); // attach to boom
	

	// restrict camera rotation
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	CharacterMovement->bOrientRotationToMovement = false;

	// Configure character movement
	CharacterMovement->GravityScale = 1.5f;
//<<<<<<< Updated upstream
	CharacterMovement->AirControl = .9f;
	CharacterMovement->JumpZVelocity = 950.0f;
//=======
	CharacterMovement->AirControl = 1.0f;
	CharacterMovement->JumpZVelocity = 800.0f;
//>>>>>>> Stashed changes
	CharacterMovement->GroundFriction = 3.0f;
	CharacterMovement->MaxWalkSpeed = 800.0f;
	CharacterMovement->MaxFlySpeed = 10.0f;

    isJumping = false;
    
	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	CharacterMovement->bConstrainToPlane = true;
	CharacterMovement->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f)); // restrict movement in y

	// Use flat floor checking as opposed to capsule base
	CharacterMovement->bUseFlatBaseForFloorChecks = true;

	// Enable replication on the Sprite component so animations show up when networked
	Sprite->SetIsReplicated(true);
	bReplicates = true;
}


void APaperPlatformerCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// bind axis
	InputComponent->BindAxis("MoveRight", this, &APaperPlatformerCharacter::MoveRight);

	// bind actions
	InputComponent->BindAction("Jump", IE_Pressed, this, &APaperPlatformerCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &APaperPlatformerCharacter::OnStopJump);
	InputComponent->BindAction("Run", IE_Pressed, this, &APaperPlatformerCharacter::OnStartRun);
	InputComponent->BindAction("Run", IE_Released, this, &APaperPlatformerCharacter::OnStopRun);
	InputComponent->BindAction("Attack", IE_Pressed, this, &APaperPlatformerCharacter::OnStartAttack);
	InputComponent->BindAction("Attack", IE_Released, this, &APaperPlatformerCharacter::OnStopAttack);
	InputComponent->BindAction("Shield", IE_Pressed, this, &APaperPlatformerCharacter::OnStartShield);
	InputComponent->BindAction("Shield", IE_Released, this, &APaperPlatformerCharacter::OnStopShield);
}

void APaperPlatformerCharacter::MoveRight(float val)
{
	if (MoveState != EMoveState::Death)
    {
        UpdateAnimation();
        if ((Controller != NULL) && (val != 0.0f))
        {
            // only moves in x axis
            const FVector Direction = FVector(1.0f, 0.0f, 0.0f);

            // change rotation of character so animation face correct direction
            if (val < 0.0f)
            {
                Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f)); // face left
            }
            else if (val > 0.0f)
            {
                Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f)); // face right
            }

            AddMovementInput(Direction, val);
        }
    }
}

void APaperPlatformerCharacter::UpdateAnimation()
{
	const FVector Velocity = GetVelocity();
	const float Speed = Velocity.Size();
	bool IsRunning = Speed > 0.0f;
	bool IsJumping = CharacterMovement->IsFalling();

	UPaperFlipbook* NextAnimation;

    if (AttackDuration > 0)
    {
        NextAnimation = IdleAttackAnimation;
    }
	else if (IsJumping)
	{
		if (BattleState == EBattleState::Shield)
		{
			NextAnimation = IdleShieldAnimation;
		}
		else
		{
			NextAnimation = JumpAnimation;
		}
	}
	else if (IsRunning)
	{
		if (BattleState == EBattleState::Shield)
		{
			NextAnimation = RunningShieldAnimation;
		}
		else
		{
			NextAnimation = RunningAnimation;
		}
	}
	else
	{
		if (BattleState == EBattleState::Shield)
		{
			NextAnimation = IdleShieldAnimation;
		}
		else
		{
			NextAnimation = IdleAnimation;
		}
	}

	Sprite->SetFlipbook(NextAnimation);
}

void APaperPlatformerCharacter::OnStartJump()
{
	Jump();
	CurrentJumps++;
	UpdateAnimation();
    isJumping= true;
}

void APaperPlatformerCharacter::OnStopJump()
{
	StopJumping();
	UpdateAnimation();
}

bool APaperPlatformerCharacter::CanJumpInternal_Implementation() const
{
	if (Super::CanJumpInternal_Implementation() || CurrentJumps <= MaxJumps) {
        JumpSoundPointer->Play();
        return true;
	}
	return false;
}

void APaperPlatformerCharacter::OnLanded(const FHitResult& Hit)
{
	CurrentJumps = 0;
    isJumping = false;
}

void APaperPlatformerCharacter::OnStartRun()
{
	if (Stamina >= StaminaRunCost)
	{
		MoveState = EMoveState::Run;
		CharacterMovement->MaxWalkSpeed = 1500.0f;
	}
}

void APaperPlatformerCharacter::OnStopRun()
{
	MoveState = EMoveState::Idle;
	CharacterMovement->MaxWalkSpeed = 800.0f;
}

void APaperPlatformerCharacter::OnStartAttack()
{
    // going to use the following function to get all actors inside the hitbox
	// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/UKismetSystemLibrary/BoxOverlapActors_NEW/index.html

    if (Stamina >= StaminaAttackCost && MoveState != EMoveState::Death)
	{
        SwordSwoosh->Activate();
        SwordSwoosh->Play();
        
        AttackDuration = 0.3f;
		
		BattleState = EBattleState::Attack;
		Stamina -= StaminaAttackCost;
	}
}

void APaperPlatformerCharacter::OnStopAttack()
{
	BattleState = EBattleState::Idle;
}

void APaperPlatformerCharacter::OnStartShield()
{
	if (Stamina >= StaminaShieldCost)
	{
		BattleState = EBattleState::Shield;
	}
}

void APaperPlatformerCharacter::OnStopShield()
{
	BattleState = EBattleState::Idle;
}


void APaperPlatformerCharacter::OnEnemyCollide(float val)
{
    /* handles being damaged */
    
	if (Health > val)
	{
		Health -= val;
	}
	else if (Health <= val && Health > 0)
    {
		Health = 0;
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You have died. Please open the menu to restart."));
        MoveState = EMoveState::Death;
	}
}

void APaperPlatformerCharacter::Tick(float DeltaSeconds)
{
    EBattleState::State OriginalState = BattleState;
    
    if (AttackDuration >= DeltaSeconds)
    {
        AttackDuration -= DeltaSeconds;
        
        TArray<AActor*> EnemiesInRange;
        
        FVector BoxPos = Hitbox->GetComponentLocation();
        FVector BoxExtent = Hitbox->GetScaledBoxExtent();
        const TArray<TEnumAsByte<EObjectTypeQuery>> filter; //not used
        const TArray<AActor*> ignore; //not used
        
        UKismetSystemLibrary::BoxOverlapActors_NEW(GetWorld(), BoxPos, BoxExtent, filter, APaperEnemy::StaticClass(), ignore, EnemiesInRange);
        
        // iterate through overlapped enemies
        for (auto &overlapped : EnemiesInRange)
        {
            // We know that all returned actors are going to be PaperEnemies
            // Here is where we will do any battle calculations, e.g. add sword atkpwr etc
            APaperEnemy* Enemy = Cast<APaperEnemy>(overlapped);
            if (Enemy)
            {
                Enemy->ReceiveDamage(AttackPower * DeltaSeconds);
            }
            
            if (Cast<APaperEnemy>(overlapped)->Health <= 0.0f)
            {
                Experience += Cast<APaperEnemy>(overlapped)->ExperienceValue;
                if (Experience >= MaxExperience)
                {
                    Level += 1;
                    Experience = 0;
                    MaxExperience += MaxExperienceIncrease;
                    BaseAttackPower += AttackPowerIncrease;
                    SaveGame();
                    
                }
            }
        }
        
    }
    else
    {
        AttackDuration = 0.0f;    }

    if (AttackBuffDuration >= DeltaSeconds)
    {
        AttackPower = BaseAttackPower * 2.5;
        AttackBuffDuration -= DeltaSeconds;
    }
    else
    {
        AttackBuffDuration = 0.0f;
        AttackPower = BaseAttackPower;
    }
    
    
    // if dead, stop character
    if (MoveState == EMoveState::Death)
    {
        TurnOff();
    }
	// if sprinting and moving
	else if ((MoveState == EMoveState::Run) && (GetVelocity().Size() > 0.0f))
	{
		if (Stamina >= StaminaRunCost) // if can sprint
		{
			Stamina -= StaminaRunCost;
		}
		else
		{
			OnStopRun();
		}
	}
	else // if not sprinting or not moving
	{
		OnStopRun();
	}

	if ((MoveState == EMoveState::Idle) && (Stamina < MaxStamina) && (BattleState == EBattleState::Idle)) // stamina regen
	{
		Stamina += StaminaRegen * DeltaSeconds;
		Stamina = (Stamina > MaxStamina) ? MaxStamina : Stamina;
	}
    
	if (BattleState == EBattleState::Shield)
	{
		if (Stamina >= StaminaShieldCost)
		{
			Stamina -= StaminaShieldCost;
		}
		else
		{
			BattleState = EBattleState::Idle;
		}
	}
    
    if (CharacterMovement->MovementMode == MOVE_Walking && GetVelocity().Size() > 0.0f)
    {
        if (!MovingSoundPointer->IsPlaying())
        {
            MovingSoundPointer->Play();
        }
    }
    else
    {
        if (MovingSoundPointer->IsPlaying())
        {
            MovingSoundPointer->Stop();
        }
    }
}

void APaperPlatformerCharacter::OnItemPickup(float BoostValue, EBoostType::Type BoostType)
{
	switch (BoostType)
	{
        case (EBoostType::HP) :
            Health += BoostValue;
            if (Health >= MaxHealth){
                Health = MaxHealth;
            }
            break;
        case (EBoostType::Stamina) :
            Stamina += BoostValue * 100;
            if (Stamina >= MaxStamina)
            {
                Stamina = MaxStamina;
            }
            break;
        case (EBoostType::Attack) :
            AttackBuffDuration += 20;
            break;
        case (EBoostType::Jump) :
            if (MaxJumps < 3) 
            {
                MaxJumps += 1;
            }
            else
            {
                MaxJumps = 3;
            }
            break;
        case (EBoostType::Victory) :
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You've won the game! Open the menu to play again :)"));
            MoveState = EMoveState::Death;
            break;
        default:
            break;
	}
}

// Set the properties we want to save and save those to disk, using the SaveGameInstance
void APaperPlatformerCharacter::SaveGame()
{
    UCSaveGame* SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
    SaveGameInstance->MaxHealth = MaxHealth;
    SaveGameInstance->MaxStam = MaxStamina;
    SaveGameInstance->BaseAttackPower = BaseAttackPower;
    SaveGameInstance->MaxJumps = MaxJumps;
    SaveGameInstance->MaxXP = MaxExperience;
    SaveGameInstance->CurrentXP = Experience;
    SaveGameInstance->CurrentLevel = Level;
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

// Return true if loadgame succeeds
bool APaperPlatformerCharacter::LoadGame()
{
    //Load Game Instance from disk
    UCSaveGame* LoadGameInstance = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
    LoadGameInstance = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
    //Restore values if the load succeeds
    if (LoadGameInstance)
    {
        MaxHealth = LoadGameInstance->MaxHealth;
        MaxStamina = LoadGameInstance->MaxStam;
        BaseAttackPower = LoadGameInstance->BaseAttackPower;
        AttackPower = LoadGameInstance->BaseAttackPower;
        MaxJumps = LoadGameInstance->MaxJumps;
        MaxExperience = LoadGameInstance->MaxXP;
        Experience = LoadGameInstance->CurrentXP;
        Level = LoadGameInstance->CurrentLevel;
        return true;
    }
    // Load Failed. Most likely no save file exists
    return false;
}

void APaperPlatformerCharacter::ClearSaveData()
{
    MaxHealth = 10;
    MaxStamina = 1000.0f;
    AttackPower = BaseAttackPower = 10.0f;
    MaxJumps = 1;
    MaxExperience = 100;
    Experience = 0;
    Level = 1;
}

