// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerAnim.h"

// Sets default values
APlayerableCharacter::APlayerableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Set AnimInstance in Mesh
		ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("AnimBlueprint'/Game/Blueprint/ABP_PlayerAnim.ABP_PlayerAnim_C'"));
		if (TempAnim.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(TempAnim.Class);
		}
	}

	// Don't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character moves in the direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.0f;

	// Camera Initialize
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERABOOM"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = m_fCameraBoomLength;
	// Rotate the Arm based on the Controller
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Camera doesn't rotate relative to arm
	Camera->bUsePawnControlRotation = false;

	m_iMaxCombo = 3;
	ResetAttackComboState();
}

// Called when the game starts or when spawned
void APlayerableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerableCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PlayerAnim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());
	RCHECK(PlayerAnim != nullptr);
	PlayerAnim->SetMaxComboIndex(m_iMaxCombo);

	PlayerAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		RLOG(Warning, TEXT("OnNextAttackCheck"));
		m_bCanNextCombo = false;

		if(m_bIsComboInputOn)
		{
			SetNextAttackCombo();
			PlayerAnim->JumpToAttackSection(m_iCurrentCombo);
		}
	});

	PlayerAnim->OnMontageEnded.AddDynamic(this, &APlayerableCharacter::OnAttackMontageEnded);

}

// Called to bind functionality to input
void APlayerableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Binding MovingFunctions 
	PlayerInputComponent->BindAxis("MoveVertical", this, &APlayerableCharacter::MoveVertical);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &APlayerableCharacter::MoveHorizontal);

	// Binding RotateFunctions
	PlayerInputComponent->BindAxis("TurnVertical", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnHorizontal", this, &APawn::AddControllerYawInput);

	// Binding Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerableCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Binding Attack
	PlayerInputComponent->BindAction("Attack",IE_Pressed, this, &APlayerableCharacter::Attack);
}

void APlayerableCharacter::MoveVertical(float fValue)
{
	if(m_bIsAttacking == true)
		return;

	if (Controller != nullptr && fValue != 0.0f)
	{
		// Find out which way is forward
		const FRotator CameraRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, CameraRotation.Yaw, 0.0f);

		// Get Forward Vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, fValue);
	}
}

void APlayerableCharacter::MoveHorizontal(float fValue)
{
	if (m_bIsAttacking == true)
		return;

	if (Controller != nullptr && fValue != 0.0f)
	{
		// Find out which way is right
		const FRotator CameraRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, CameraRotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, fValue);
	}
}

void APlayerableCharacter::Jump()
{
	if(m_bIsAttacking)
		return;

	ACharacter::Jump();
}

void APlayerableCharacter::Attack()
{
	if(GetCharacterMovement()->IsFalling())
		return;

	if(m_bIsAttacking)
	{
		RCHECK(FMath::IsWithinInclusive<int32>(m_iCurrentCombo,1,m_iMaxCombo));
		m_bIsComboInputOn = true;
	}
	else
	{
		RCHECK(m_iCurrentCombo == 0);
		SetNextAttackCombo();
		PlayerAnim->PlayAttackAnim();
		PlayerAnim->JumpToAttackSection(m_iCurrentCombo);
		m_bIsAttacking = true;
	}
}

void APlayerableCharacter::SetNextAttackCombo()
{
	m_bCanNextCombo = true;
	m_bIsComboInputOn = false;
	RCHECK(FMath::IsWithinInclusive<int32>(m_iCurrentCombo, 0, m_iMaxCombo-1));
	m_iCurrentCombo = FMath::Clamp<int32>(m_iCurrentCombo + 1, 1, m_iMaxCombo);
}

void APlayerableCharacter::ResetAttackComboState()
{
	m_bIsComboInputOn = false;
	m_bIsAttacking = false;
	m_bCanNextCombo = false;
	m_iCurrentCombo = 0;
}

void APlayerableCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	RCHECK(m_bIsAttacking == true);
	RCHECK(m_iCurrentCombo > 0);
	ResetAttackComboState();
}
