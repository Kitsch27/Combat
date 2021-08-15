// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SetGravityScale.h"

#include "Combat/Character/CombatCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UAN_SetGravityScale::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	ACombatCharacter* Character = Cast<ACombatCharacter>(MeshComp->GetAttachmentRootActor());
	if (Character)
	{
		if (bChangeZVelocity)
		{
			Character->GetCharacterMovement()->Velocity.Z = 0.f;
		}
		Character->GetCharacterMovement()->GravityScale = GravityScaleRate;
	}
}
