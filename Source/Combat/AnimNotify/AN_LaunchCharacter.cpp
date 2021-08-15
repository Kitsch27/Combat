// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_LaunchCharacter.h"

#include "Combat/Character/CombatCharacter.h"

void UAN_LaunchCharacter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Actor = MeshComp->GetOwner();
	ACombatCharacter* Character = Cast<ACombatCharacter>(Actor);
	if (Character == nullptr)
		return;
	
	Character->LaunchCharacter(LaunchVelocity * 1000, false, false);
}
