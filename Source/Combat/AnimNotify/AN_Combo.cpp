// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_Combo.h"

#include "Combat/Character/CombatPlayer.h"

void UAN_Combo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ACombatPlayer* Player = Cast<ACombatPlayer>(MeshComp->GetAttachmentRootActor());
	if (Player && ComboMontage)
	{
		Player->SetComboMontage(ComboMontage);
		Player->SetCanChangeAttack(true);
	}
}
