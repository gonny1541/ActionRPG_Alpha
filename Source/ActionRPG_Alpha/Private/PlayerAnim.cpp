// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnim::UPlayerAnim()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(
	TEXT("AnimMontage'/Game/Blueprint/ABP_AttackMontage.ABP_AttackMontage'"));
	if(ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto owner = TryGetPawnOwner();
	auto player = Cast<ACharacter>(owner);

	RCHECK(player != NULL);
	UCharacterMovementComponent* moveComp = player->GetCharacterMovement();

	RCHECK(moveComp != NULL);
	m_fSpeed = moveComp->GetCurrentAcceleration().Length();
	m_fZVelocity = moveComp->Velocity.Z;
	m_bisAir = moveComp->IsFalling();
}

void UPlayerAnim::SetMaxComboIndex(int32 iMaxComboIndex)
{
	(iMaxComboIndex > 1) ? (m_iMaxComboIndex = iMaxComboIndex) : (m_iMaxComboIndex = 1);
}

void UPlayerAnim::PlayAttackAnim()
{
	if (Montage_IsPlaying(AttackMontage) != true)
	{
		RCHECK(Montage_Play(AttackMontage, 1.0f) != 0.f);
	}
}

void UPlayerAnim::JumpToAttackSection(int32 iSectionIndex)
{
	RCHECK(Montage_IsPlaying(AttackMontage));
	RCHECK(FMath::IsWithinInclusive<int32>(iSectionIndex, 1, m_iMaxComboIndex));
	FName strIndexName = FName(*FString::Printf(TEXT("Attack%d"), iSectionIndex));
	Montage_JumpToSection(strIndexName, AttackMontage);
}

void UPlayerAnim::AnimNotify_CheckNextAttack()
{
	OnNextAttackCheck.Broadcast();
}
