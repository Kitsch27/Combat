// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_LockAttack.h"

#include "Combat/Character/CombatPlayer.h"

void UAN_LockAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ACombatPlayer* Player = Cast<ACombatPlayer>(MeshComp->GetAttachmentRootActor());
	if (Player)
	{
		Player->SetCanChangeAttack(bState);
	}
}
