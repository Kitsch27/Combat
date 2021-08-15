// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DT_Montage.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UDT_Montage : public UDataTable
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
enum class EMontageType:uint8
{
	E_Hit,
	E_HitSkill
};

/** 出招指令 */
UENUM(BlueprintType)
enum class EInputBufferingType:uint8
{
	E_Front UMETA(DisplayName = "Front"),
	E_DoubleFront UMETA(DisplayName = "DoubleFront"),
	E_HoldFront UMETA(DisplayName = "HoldFront"),
	E_Back UMETA(DisplayName = "Back"),
	E_DoubleBack UMETA(DisplayName = "DoubleBack"),
	E_HoldBack UMETA(DisplayName = "HoldBack"),
	E_Left UMETA(DisplayName = "Left"),
	E_DoubleLeft UMETA(DisplayName = "DoubleLeft"),
	E_HoldLeft UMETA(DisplayName = "HoldLeft"),
	E_Right UMETA(DisplayName = "Right"),
	E_DoubleRight UMETA(DisplayName = "DoubleRight"),
	E_HoldRight UMETA(DisplayName = "HoldRight"),
	E_Circle UMETA(DisplayName = "Circle"),
	E_HalfCircle UMETA(DisplayName = "HalfCircle"),
	E_NormalAttack UMETA(DisplayName = "NormalAttack"),
	E_ChargeNormalAttack UMETA(DisplayName = "ChargeNormalAttack"),
	E_SpecialAttack UMETA(DisplayName = "SpecialAttack"),
	E_ChargeSpecialAttack UMETA(DisplayName = "ChargeSpecialAttack"),
	E_Jump UMETA(DisplayName = "Jump"),
	E_Dodge UMETA(DisplayName = "Dodge"),
};

/** Montage类型 */
UENUM(BlueprintType)
enum class EMontageInfoType:uint8 
{
	E_GetHit UMETA(DisplayName = "GetHit"),
	E_Jump UMETA(DisplayName = "Jump"),
	E_Attack UMETA(DisplayName = "Attack"),
	E_Skill UMETA(DisplayName = "Skill"),
	E_Other UMETA(DisplayName = "Other"),
	E_Dodge UMETA(DisplayName = "Dodge"),
};

// 武器
UENUM(BlueprintType)
enum class EWeaponType:uint8
{
	E_Sword UMETA(DisplayName = "Sword"),
};

/** 技能指令 */
USTRUCT(BlueprintType, Blueprintable)
struct FSkillCommand :public FTableRowBase
{
	GENERATED_BODY()

	// 出招指令
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputBufferingType InputBufferingType;

	// 有效帧长度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ValidFrameLength;

};

/** Montage信息*/
USTRUCT(BlueprintType, Blueprintable)
struct FMontageInfo :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	/** 类型 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMontageInfoType MontageInfoType;

	/** 适用武器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EWeaponType> WeaponTypeList;

	/** 在空中 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInAir;

	/** 在地面 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOnGround;

	/** 锁定 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLock;

	/** 出招表 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillCommand> CommandList;

	/** 对应动画蒙太奇 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;

	/** 优先级 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Priority;
	
	/** 有效性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bValid;

	/** 备注 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Remarks;
	
};