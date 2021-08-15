// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_AttackEnd.h"

#include "Combat/Character/CombatCharacter.h"
#include "Combat/Character/CombatPlayer.h"

void UAN_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACombatCharacter* Character = Cast<ACombatCharacter>(MeshComp->GetAttachmentRootActor());
	if (Character)
	{
		// 改变攻击状态
		Character->SetAttacking(false);

		ACombatPlayer* Player = Cast<ACombatPlayer>(MeshComp->GetAttachmentRootActor());
		if (Player)
		{
			Player->SetComboMontage(nullptr);
			Player->SetCanChangeAttack(true);
		}
	}
}
