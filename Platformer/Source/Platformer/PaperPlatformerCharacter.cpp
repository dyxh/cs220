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
	// enable tick
	PrimaryActorTick.bCanEverTick = true;

	// set baseline health/stamina
	Health = MaxHealth = 1000.0f;
	Stamina = MaxStamina = 1000.0f;

	HealthRegen = 0.0f;
	StaminaRegen = 0.010f;

	AttackPower = 500.0f;
	AttackPowerIncrease = 100.0f;

	Experience = 0;
	MaxExperience = 100;
	MaxExperienceIncrease = 50;
	Level = 1;

	StaminaRunCost = 10.0f;
	StaminaShieldCost = 10.0f;
	StaminaAttackCost = 100.0f;

	MaxJumps = 3;
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
		FConstructorStatics()
			: RunningAnimationAsset(TEXT("/Game/Flipbooks/PlayerRunning.PlayerRunning")) ///Game/KenneySpritePack/Players/Green/alienGreen_walk.alienGreen_walk
			, IdleAnimationAsset(TEXT("/Game/Flipbooks/PlayerIdle.PlayerIdle")) //KenneySpritePack/Players/Green/alienGreen_idle.alienGreen_idle
			, JumpAnimationAsset(TEXT("/Game/Flipbooks/PlayerJump.PlayerJump"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	RunningAnimation = ConstructorStatics.RunningAnimationAsset.Get();
	IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
	JumpAnimation = ConstructorStatics.JumpAnimationAsset.Get();
	Sprite->SetFlipbook(IdleAnimation);

	// set capsule size
	CapsuleComponent->SetCapsuleHalfHeight(60.0f);
	CapsuleComponent->SetCapsuleRadius(40.0f);

	// ignore pitch and roll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Setup damage box
	DamageBox = PCIP.CreateDefaultSubobject<UBoxComponent>(this, TEXT("DamageBox"));
	DamageBox->SetBoxExtent(FVector(40.0f, 0.0f, 60.0f)); // extends 40 units from the character
	DamageBox->AttachTo(RootComponent);
	DamageBox->RelativeLocation = FVector(40.0f, 0.0f, 0.0f);

	// attach camera boom to capsule
	CameraBoom = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->RelativeRotation = FRotator(0.5, -90.f, 0.f);

	// Create and attach orthographic cam to boom
	SideViewCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("2DViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 1024.0f;
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName); // attach to boom

	// restric camera rotation
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	CharacterMovement->bOrientRotationToMovement = false;

	// Configure character movement
	CharacterMovement->GravityScale = 2.0f;
	CharacterMovement->AirControl = 0.80f;
	CharacterMovement->JumpZVelocity = 1000.f;
	CharacterMovement->GroundFriction = 3.0f;
	CharacterMovement->MaxWalkSpeed = 800.0f;
	CharacterMovement->MaxFlySpeed = 600.0f;

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

void APaperPlatformerCharacter::UpdateAnimation()
{
	const FVector Velocity = GetVelocity();
	const float Speed = Velocity.Size();
	bool IsRunning = Speed > 0.0f;
	bool IsJumping = CharacterMovement->IsFalling();

	UPaperFlipbook* NextAnimation;

	if (IsJumping)
	{
		NextAnimation = JumpAnimation;
	}
	else if (IsRunning)
	{
		NextAnimation = RunningAnimation;
	}
	else
	{
		NextAnimation = IdleAnimation;
	}

	Sprite->SetFlipbook(NextAnimation);
}

void APaperPlatformerCharacter::OnStartJump()
{
	Jump();
	CurrentJumps++;
	UpdateAnimation();
}

void APaperPlatformerCharacter::OnStopJump()
{
	StopJumping();
	UpdateAnimation();
}

bool APaperPlatformerCharacter::CanJumpInternal_Implementation() const
{
	if (Super::CanJumpInternal_Implementation() || CurrentJumps <= MaxJumps) {
		return true;
	}
	return false;
}

void APaperPlatformerCharacter::OnLanded(const FHitResult& Hit)
{
	CurrentJumps = 0;
}

void APaperPlatformerCharacter::OnStartRun()
{
	if (Stamina > StaminaRunCost)
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
	// going to use the following function to get all actors inside the damagebox
	// https://docs.unrealengine.com/latest/INT/API/Runtime/Engine/Kismet/UKismetSystemLibrary/BoxOverlapActors_NEW/index.html
	if (Stamina > 200.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ATTACK!"));
		BattleState = EBattleState::Attack;
		Stamina -= StaminaAttackCost;

		TArray<AActor*> EnemiesInRange;

		FVector BoxPos = DamageBox->GetComponentLocation();
		FVector BoxExtent = DamageBox->GetScaledBoxExtent();
		const TArray<TEnumAsByte<EObjectTypeQuery>> filter; //not used
		const TArray<AActor*> ignore; //not used

		UKismetSystemLibrary::BoxOverlapActors_NEW(GetWorld(), BoxPos, BoxExtent, filter, APaperEnemy::StaticClass(), ignore, EnemiesInRange);

		// iterate through overlapped enemies
		for (auto &overlapped : EnemiesInRange)
		{
			// We know that all returned actors are going to be PaperEnemies
			// Here is where we will do any battle calculations, e.g. add sword atkpwr etc
			Cast<APaperEnemy>(overlapped)->ReceiveDamage(AttackPower);
			if (Cast<APaperEnemy>(overlapped)->Health <= 0.0f)
			{
				Experience += Cast<APaperEnemy>(overlapped)->ExperienceEnemy;
				if (Experience >= MaxExperience)
				{
					Level += 1;
					Experience = 0;
					MaxExperience += MaxExperienceIncrease;
				}
			}
		}
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("SHIELD!"));
		BattleState = EBattleState::Shield;
	}
}

void APaperPlatformerCharacter::OnStopShield()
{
	BattleState = EBattleState::Idle;
}

// Handles taking damage
void APaperPlatformerCharacter::OnEnemyCollide(float val)
{
	if (Health >= val)
	{
		Health -= val;
	}
	else
	{
		Health = 0;
		GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Red, TEXT("U LOSE SUCKA!!"));
	}
}

void APaperPlatformerCharacter::Tick(float DeltaSeconds)
{
	// if trying to run and actually moving
	if ((MoveState == EMoveState::Run) && (GetVelocity().Size() > 0.0f))
	{
		if (Stamina >= StaminaRunCost)
		{
			Stamina -= StaminaRunCost;
		}
		else
		{
			OnStopRun();
		}
	}
	else // if not moving
	{
		OnStopRun();
	}

	if ((MoveState == EMoveState::Idle) && (Stamina < MaxStamina) && (BattleState == EBattleState::Idle)) // stamina regen
	{
		Stamina += MaxStamina * StaminaRegen;
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
}

void APaperPlatformerCharacter::OnItemPickup(float boost, EnumType::bType type)
{
	switch (type) {
	case (EnumType::HP) :
		Health += boost;
		if (Health >= MaxHealth){
			Health = MaxHealth;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HP boost!"));
		break;
	case(EnumType::Stamina) :
		Stamina += boost;
		if (Stamina >= MaxStamina){
			Stamina = MaxStamina;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Stamina boost!"));
		break;
		//    case(EnumType::Attack):
		//        AttackPower += boost;
		//            if (AttackPower >= MaxAttackPower){
		//                AttackPower = MaxAttackPower;
		//            }
		//        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Attack boost!"));
		break;
	default:
		break;
	}
}

void APaperPlatformerCharacter::SaveGame()
{
    
    UCSaveGame* SaveGameInstance = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
    SaveGameInstance->MaxHealth = MaxHealth;
    SaveGameInstance->MaxStam = MaxStamina;
    SaveGameInstance->Attack = AttackPower;
    SaveGameInstance->MaxJumps = MaxJumps;
    SaveGameInstance->MaxXP = MaxExperience;
    SaveGameInstance->CurrentXP = Experience;
    SaveGameInstance->CurrentLevel = Level;
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void APaperPlatformerCharacter::LoadGame()
{
    UCSaveGame* LoadGameInstance = Cast<UCSaveGame>(UGameplayStatics::CreateSaveGameObject(UCSaveGame::StaticClass()));
    LoadGameInstance = Cast<UCSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
    MaxHealth = LoadGameInstance->MaxHealth;
    MaxStamina = LoadGameInstance->MaxStam;
    AttackPower = LoadGameInstance->Attack;
    MaxJumps = LoadGameInstance->MaxJumps;
    MaxExperience = LoadGameInstance->MaxXP;
    Experience = LoadGameInstance->CurrentXP;
    Level = LoadGameInstance->CurrentLevel;
    
}