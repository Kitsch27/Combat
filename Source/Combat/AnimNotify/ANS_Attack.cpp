// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Attack.h"

#include "Combat/CombatDestructibleActor.h"
#include "Combat/Character/CombatEnemy.h"
#include "Kismet/KismetSystemLibrary.h"


void UANS_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	Character = Cast<ACombatCharacter>(MeshComp->GetAttachmentRootActor());
	if (IsValid(Character))
	{
		if (bUseHitBox)
		{
			if (!IsValid(Character->HitBox))
				return;
		}
		else
		{
			Weapon = Character->WeaponSkeletalMeshComponent;
			if (!IsValid(Weapon))
				return;

			if (SocketArr.Num() == 0)
				SocketArr = Weapon->GetAllSocketNames();
			
			Density = fmax(fmax(SocketArr.Num(), Density), 2);
		}
		LocationArr = GetLocationArr();
		IgnoreActorArr = { MeshComp->GetOwner() };
	}
}

void UANS_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	if (!IsValid(Character))
	{
		return;
	}
	TArray<FVector> CurrentLocationArr = GetLocationArr();
	if (bUseHitBox)
	{
		// 使用hitBox检测
		if (!IsValid(Character->HitBox))
			return;
		for (int i = 0; i < LocationArr.Num(); ++i)
		{
			TArray<FHitResult> HitResult;
			UKismetSystemLibrary::BoxTraceMulti(Character->GetWorld(), LocationArr[i], CurrentLocationArr[i],
			                                    Character->HitBox->GetScaledBoxExtent(),
			                                    Character->HitBox->GetComponentRotation(),
			                                    TraceTypeQuery4, false, IgnoreActorArr,
			                                    EDrawDebugTrace::None, HitResult, true);
			DealHitResult(HitResult);
		}
	}
	else
	{
		// 使用射线检测
		if (!IsValid(Weapon))
			return;

		for (int i = 0; i < LocationArr.Num(); ++i)
		{
			TArray<FHitResult> HitResult;
			UKismetSystemLibrary::LineTraceMulti(Character->GetWorld(), LocationArr[i], CurrentLocationArr[i],
			                                     TraceTypeQuery4, false, IgnoreActorArr,
			                                     EDrawDebugTrace::None, HitResult, true);
			DealHitResult(HitResult);
		}
	}
	LocationArr = CurrentLocationArr;
}

void UANS_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	LocationArr.Empty();
	IgnoreCmpArr.Empty();
	IgnoreActorArr.Empty();
}

TArray<FVector> UANS_Attack::GetLocationArr()
{
	TArray<FVector> Arr;
	if (bUseHitBox)
	{
		if (IsValid(Character) && IsValid(Character->HitBox))
			return {Character->HitBox->GetComponentLocation()};
	}
	else
	{
		if (SocketArr.Num() > 0)
		{
			// 从起始点到末尾点，根据Density划分位置
			for (int i = 0; i < SocketArr.Num() - 1; ++i)
			{
				FVector Start = Weapon->GetSocketLocation(SocketArr[i]);
				FVector End = Weapon->GetSocketLocation(SocketArr[i + 1]);
				FVector Distance = End - Start;
				FVector Interval = Distance / (Density - 1);
				for (int j = 0; j < Density; ++j)
				{
					Arr.Add(Start + Interval * j);
				}
			}
		}
	}

	return Arr;
}

void UANS_Attack::DealHitResult(TArray<FHitResult>& HitResult)
{
	for (FHitResult CurrentHitResult : HitResult)
	{
		AActor* HitActor = CurrentHitResult.GetActor();

		if (HitActor->Tags.Contains("Destructible"))
		{
			HitDestructible(HitActor, -CurrentHitResult.ImpactNormal.GetSafeNormal());
		}
		else if (HitActor->Tags.Contains("Character"))
		{
			HitCharacter(CurrentHitResult);
		}
		
	}
}

void UANS_Attack::HitDestructible(AActor* HitActor, FVector ImpulseDir)
{
	ACombatDestructibleActor* DestructibleActor =  Cast<ACombatDestructibleActor>(HitActor);
	if (DestructibleActor == nullptr)
		return;
	DestructibleActor->OnGetHit(HitActor->GetActorLocation(), ImpulseDir);
	
	// 调用Character的攻击方法
	AttackDamage.SlomoRate = 0.05f;
	AttackDamage.SlomoTime = 0.1f;
	Character->Hit(HitActor, AttackDamage);
	IgnoreActorArr.Add(HitActor); //已经被击中的加入忽略数组
}

void UANS_Attack::HitCharacter(FHitResult CurrentHitResult)
{
	AActor* HitActor = CurrentHitResult.GetActor();
	// 拼刀
	UPrimitiveComponent* Cmp = CurrentHitResult.GetComponent();
	if (Cmp != nullptr && Cmp->GetName() == "HitBox")
	{
		if (!IgnoreCmpArr.Contains(Cmp))
		{
			ACombatEnemy* CombatEnemy = Cast<ACombatEnemy>(HitActor);
			if (bCanParry && CombatEnemy != nullptr && CombatEnemy->IsAttacking())
			{
				const FVector GetHitDirection = (AttackDamage.KnockBackType == EHitKnockBackType::E_HitActor)
													? (CombatEnemy->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal()
													: -CurrentHitResult.ImpactNormal;
				CombatEnemy->BeParried(Character, CurrentHitResult.ImpactPoint, GetHitDirection, AttackDamage);
			}
			IgnoreCmpArr.Add(Cmp); //已经被击中的加入忽略数组
		}
		return;
	}

	ACombatCharacter* HitCharacter = Cast<ACombatCharacter>(HitActor);
	if (HitCharacter != nullptr)
	{
		const FCharacterState CharacterState = HitCharacter->GetCharacterState();
		if (CharacterState.bIsInvincible)
		{
			// 无敌状态不受攻击
			return;
		}
		// 计算击中方向
		const FVector GetHitDirection = (AttackDamage.KnockBackType == EHitKnockBackType::E_HitActor)
											? (HitCharacter->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal()
											: -CurrentHitResult.ImpactNormal;

		// 调用受击方法
		HitCharacter->GetHit(Character, CurrentHitResult.ImpactPoint, GetHitDirection, AttackDamage);
	}
	// 调用Character的攻击方法
	Character->Hit(HitActor, AttackDamage);
	IgnoreActorArr.Add(HitActor); //已经被击中的加入忽略数组
}