// Fill out your copyright notice in the Description page of Project Settings.

#include "Platformer.h"
#include "PaperPlatformerCharacter.h"
#include "PaperFlipbookComponent.h"


APaperPlatformerCharacter::APaperPlatformerCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// enable tick
	PrimaryActorTick.bCanEverTick = true;

	// set baseline health/stamina
	Health = MaxHealth = 1000.0f;
	Stamina = MaxStamina = 1000.0f;

	HealthRegen = 0.0f;
	StaminaRegen = 0.001f;

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
			: RunningAnimationAsset(TEXT("/Game/KenneySpritePack/Players/Green/alienGreen_walk.alienGreen_walk"))
			, IdleAnimationAsset(TEXT("/Game/KenneySpritePack/Players/Green/alienGreen_idle.alienGreen_idle"))
			, JumpAnimationAsset(TEXT("/Game/KenneySpritePack/Players/Green/alienGreen_jump.alienGreen_jump"))
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

	//ignore pitch and roll
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
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Run", IE_Pressed, this, &APaperPlatformerCharacter::OnStartRun);
	InputComponent->BindAction("Run", IE_Released, this, &APaperPlatformerCharacter::OnStopRun);
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
	UpdateAnimation();
}

void APaperPlatformerCharacter::OnStopJump()
{
	StopJumping();
	UpdateAnimation();
}

void APaperPlatformerCharacter::OnStartRun()
{
	if (Stamina > 0)
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

void APaperPlatformerCharacter::Tick(float DeltaSeconds)
{

	if ((MoveState == EMoveState::Run) && (GetVelocity().Size() > 0.0f))
	{
		if (Stamina > 0)
		{
			Stamina -= 10.0f;
		}
		else
		{
			OnStopRun();
		}
	}
	
	if ((MoveState == EMoveState::Idle) && (Stamina < MaxStamina)) // stamina regen
	{
		Stamina += MaxStamina * StaminaRegen;
		Stamina = (Stamina > MaxStamina) ? MaxStamina : Stamina;
	}
}