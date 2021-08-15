// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_CameraShake.h"

#include "Kismet/GameplayStatics.h"

void UAN_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	
	if (!IsValid(CamShake))
		return;
	AActor* Actor = MeshComp->GetAttachmentRootActor();
	FVector FinalEpicenter = Actor->GetActorLocation() + Epicenter;
	UGameplayStatics::PlayWorldCameraShake(Actor->GetWorld(), CamShake, FinalEpicenter, InnerRadius, OuterRadius, Falloff, false);
}
