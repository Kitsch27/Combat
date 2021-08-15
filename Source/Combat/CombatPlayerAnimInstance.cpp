// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayerAnimInstance.h"

#include "Character/CombatCharacter.h"
#include "Character/CombatPlayer.h"
#include "GameFramework/PawnMovementComponent.h"

void UCombatPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCombatPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Owner = TryGetPawnOwner();
	if (IsValid(Owner))
	{
		// 获取水平移动速度
		Velocity = Owner->GetVelocity();
		MoveSpeed = FVector(Velocity.X, Velocity.Y, 0).Size(); 
		ACombatPlayer* Player = Cast<ACombatPlayer>(Owner);

		// 获取锁定状态
		if (Player)
		{
			bIsLocking = Player->GetLockState();

			// 是否受击
			bIsGetHit = Player->IsGetHit();

			// 人物状态
			CharacterState = Player->GetCharacterState();
		}
		if (bIsLocking)
		{
			MoveForwardSpeed = Velocity | Player->GetForwardDirection();
			MoveRightSpeed = Velocity | Player->GetRightDirection();
		}
		else
		{
			MoveForwardSpeed = 0;
			MoveRightSpeed = 0;
		}

		UPawnMovementComponent* MovementComponent = Owner->GetMovementComponent();
		if (MovementComponent)
		{
			// 是否在空中
			bIsInAir = MovementComponent->IsFalling();
			bIsOnGround = !bIsInAir;
		}
	}
}
