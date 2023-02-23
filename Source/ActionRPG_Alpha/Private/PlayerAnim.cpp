// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto player = TryGetPawnOwner();

	if (player != NULL)
	{
		m_fSpeed = player->GetVelocity().Size();
	}
}
