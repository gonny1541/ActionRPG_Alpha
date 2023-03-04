// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionRPG_Lib.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);

/**
 * 
 */
UCLASS()
class ACTIONRPG_ALPHA_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnim();

	// Player Velocity
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Player)
	float m_fSpeed = 0.0f;

	// Player Z Direction Velocity
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	float m_fZVelocity = 0.0f;

	// Is Player in the air
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Player)
	bool m_bisAir = false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	//Attack
	FOnNextAttackCheckDelegate OnNextAttackCheck;

	void SetMaxComboIndex(int32 iMaxComboIndex);

	void PlayAttackAnim();

	void JumpToAttackSection(int32 iSectionIndex);

private:
	UFUNCTION()
	void AnimNotify_CheckNextAttack();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	int32 m_iMaxComboIndex;
};
