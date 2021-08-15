// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CombatGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
	
	virtual void Shutdown() override;
	
	virtual void StartGameInstance() override;

public:
	// 显示debug信息
	bool bShowDebugMsg = false;
	// 显示debug绘制
	bool bShowDebugDraw = false;
};
