// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_RotationToTarget.h"

#include "Combat/Character/CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UANS_RotationToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	
	Self = Cast<ACombatCharacter>(MeshComp->GetAttachmentRootActor());
	if (!IsValid(Self))
		return;
	
	// 获取最近的Character
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(Self->GetWorld(), ACombatCharacter::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		// 移除自己
		if (Actor == Self)
			continue;

		// 移除大于锁定范围的对象
		const float Distance = Self->GetDistanceTo(Actor);
		if (Distance > LockDistance)
			continue;
		
		if (Target == nullptr)
			Target = Actor;

		const float TargetDistance = Self->GetDistanceTo(Target);
		if (Distance < TargetDistance)
			Target = Actor;
	}
}

void UANS_RotationToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	
	if (!IsValid(Target))
		return;
	const FRotator CurrentRotator = Self->GetActorRotation();
	const FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(Self->GetActorLocation(), Target->GetActorLocation());
	const FRotator ActorRotation = FMath::RInterpTo(CurrentRotator, FRotator(CurrentRotator.Pitch, TargetRotator.Yaw, CurrentRotator.Roll), FrameDeltaTime, RotateSpeed);
	Self->SetActorRotation(ActorRotation);
}
