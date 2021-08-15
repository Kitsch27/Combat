// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyAnimInstance.h"
#include "Character/CombatCharacter.h"
#include "Character/CombatEnemy.h"


void UCombatEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCombatEnemyAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	APawn* Owner = TryGetPawnOwner();
	if (IsValid(Owner))
	{
		Velocity = Owner->GetVelocity();
		ACombatCharacter* Character = Cast<ACombatCharacter>(Owner);
		
		MoveForwardSpeed = Velocity | Character->GetForwardDirection();
		MoveRightSpeed = Velocity | Character->GetRightDirection();

		ACombatCharacter* CombatCharacter = Cast<ACombatCharacter>(Owner);
		CharacterState = CombatCharacter->GetCharacterState();
	}
}
