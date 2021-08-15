// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SetState.h"

#include "Combat/Character/CombatPlayer.h"
#include "Components/CapsuleComponent.h"

void UAN_SetState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ACombatCharacter* Character = Cast<ACombatCharacter>(MeshComp->GetAttachmentRootActor());
	if (Character)
	{
		// 改变受击状态
		if (GetHitState != EState::E_NoChange)
			Character->SetGetHit(GetHitState == EState::E_True ? true : false);
		
		// 改变闪避状态
		if (DodgeState != EState::E_NoChange)
			Character->SetDodgeState(DodgeState == EState::E_True ? true : false);

		// 改变攻击状态
		if (AttackState != EState::E_NoChange)
		{
			Character->SetAttacking(AttackState == EState::E_True ? true : false);
			ACombatPlayer* Player = Cast<ACombatPlayer>(Character);
			if (Player != nullptr)
			{
				if (AttackState == EState::E_False)
				{
					Player->SetComboMontage(nullptr);
					Player->SetCanChangeAttack(true);
				}
				else
				{
					Player->SetCanChangeAttack(false);
				}
			}
		}

		// 改变人物状态
		FCharacterState CharacterState = Character->GetCharacterState();
		if (FallDownState != EState::E_NoChange)
		{
			// 改变倒地状态
			CharacterState.bIsFallDown = FallDownState == EState::E_True ? true : false;
		}
		if (SuperArmorState != EState::E_NoChange)
		{
			// 改变霸体状态
			CharacterState.bIsSuperArmor = SuperArmorState == EState::E_True ? true : false;
		}
		if (InvincibleState != EState::E_NoChange)
		{
			// 改变无敌状态
			CharacterState.bIsInvincible = InvincibleState == EState::E_True ? true : false;
		}
		if (CanRotateState != EState::E_NoChange)
		{
			// 改变可转向状态
			CharacterState.bCanRotate = CanRotateState == EState::E_True ? true : false;
		}
		Character->SetCharacterState(CharacterState);

		// 胶囊体
		UCapsuleComponent* CapsuleCmp = Character->GetCapsuleComponent();
		if (CapsuleCmp != nullptr)
		{
			CapsuleHalfHeight = fmax(CapsuleHalfHeight, CapsuleCmp->GetUnscaledCapsuleRadius());
			if (CapsuleCmp->GetUnscaledCapsuleHalfHeight() != CapsuleHalfHeight)
				CapsuleCmp->SetCapsuleHalfHeight(CapsuleHalfHeight);
		}
	}
}
