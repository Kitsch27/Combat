// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemy.h"

#include "AIC_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ACombatEnemy::ACombatEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACombatEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyController = Cast<AAIC_Enemy>(GetController());
	if (EnemyController)
	{
		BlackBoardData = EnemyController->BlackBoardData;
	}
}


void ACombatEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterState.bIsDead)
	{
		return;
	}

	if (BlackBoardData != nullptr && EnemyController != nullptr)
	{
		EnemyController->BlackboardComp->SetValueAsBool("bIsAttacking", bIsAttacking);
		EnemyController->BlackboardComp->SetValueAsBool("bIsGetHit", bIsGetHit);
	}
}

void ACombatEnemy::Attack(UAnimMontage* AttackMontage)
{
	UAnimInstance* AnimaInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimaInstance) && !AnimaInstance->Montage_IsPlaying(AttackMontage))
	{
		bIsAttacking = true;	// 设置攻击状态
		AnimaInstance->Montage_Play(AttackMontage);	// 播放对应的Montage
	}
}

void ACombatEnemy::BeParried(AActor* AttackActor, const FVector GetHitPoint, const FVector GetHitDirection, const FAttackDamage AttackDamage)
{
	UE_LOG(LogTemp, Log, TEXT("%s : Be Parried by %s"), *this->GetName(), *AttackActor->GetName());
	if (CombatGameInstance && CombatGameInstance->bShowDebugMsg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString(this->GetName() + " : Be Parried by " + AttackActor->GetName()));
	}
	bIsGetHit = true;

	// 拼刀特效
	if (BeParriedParticle != nullptr)
	{
		const FRotator SpawnRotation = (-GetHitDirection.Rotation().RotateVector(FVector(0.f,0.f, -90.f))).Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeParriedParticle, GetHitPoint, SpawnRotation);
	}

	// 拼刀音效
	if (BeParriedSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BeParriedSound, GetHitPoint);
	}

	SlomoGlobalTimeDilation(0.2f, 0.05f);

	// 拼刀动画
	if (!CharacterState.bIsFallDown)
	{
		CharacterState.bCanRotate = false;

		UAnimInstance* AnimaInstance =  GetMesh()->GetAnimInstance();
		if (AnimaInstance && GetHitMontage)
		{
			// 播放拼刀动画
			if (!AnimaInstance->Montage_IsPlaying(GetHitMontage))
			{
				AnimaInstance->Montage_Play(GetHitMontage);
			}
			// 根据受击方向，跳转对应的受击动画部分
			FName HitSection = "Front";
			// 无效时选择首个Section
			if (!GetHitMontage->IsValidSectionName(HitSection))
			{
				HitSection = GetHitMontage->GetSectionName(0);
			}
			AnimaInstance->Montage_JumpToSection(HitSection);
		}
	}
}

void ACombatEnemy::UpdateHealth(float Value)
{
	Super::UpdateHealth(Value);

	if (CharacterState.bIsDead)
		DetachFromControllerPendingDestroy();
}
