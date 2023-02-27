// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto owner = TryGetPawnOwner();
	auto player = Cast<ACharacter>(owner);

	if (player != NULL)
	{
		UCharacterMovementComponent* moveComp = player->GetCharacterMovement();

		m_fSpeed = moveComp->GetCurrentAcceleration().Length();
		m_fZVelocity = moveComp->Velocity.Z;
		m_bisAir = moveComp->IsFalling();
	}
}
