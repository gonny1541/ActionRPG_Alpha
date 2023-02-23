// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONRPG_ALPHA_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	// Player Velocity
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Player)
	float m_fSpeed = 0.0f;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
