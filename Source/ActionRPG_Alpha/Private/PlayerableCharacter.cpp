// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerableCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	}

	// Don't rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Character moves in the direction of input
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

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
}

void APlayerableCharacter::MoveVertical(float fValue)
{
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
	if (Controller != nullptr && fValue != 0.0f)
	{
		// Find out which way is right
		const FRotator CameraRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, CameraRotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, fValue);
	}
}

