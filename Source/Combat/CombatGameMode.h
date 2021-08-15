// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/CombatEnemy.h"
#include "Character/CombatPlayer.h"
#include "GameFramework/GameMode.h"
#include "Widget/UW_Player.h"

#include "CombatGameMode.generated.h"

// 动态代理 多播
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDelegate);

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatGameMode : public AGameMode
{
	GENERATED_BODY()


	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;


public:
	virtual void ShowRestartWidget();

	UPROPERTY(BlueprintAssignable)
	FGameOverDelegate OnGameOverDelegate;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> EnemyWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<UUserWidget> RestartWidgetClass;
	
private:
	UPROPERTY()
	UUserWidget*  PlayerWidget;

	UPROPERTY()
	UUserWidget*  EnemyWidget;
	
	UPROPERTY()
	UUserWidget*  RestartWidget;

	UPROPERTY()
	ACombatPlayer* CombatPlayer;

	UPROPERTY()
	TArray<ACombatEnemy*> CombatEnemyArr;
	
};
