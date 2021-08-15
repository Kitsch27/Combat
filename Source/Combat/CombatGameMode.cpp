// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ACombatGameMode::StartPlay()
{
	Super::StartPlay();

	if (PlayerWidgetClass)
	{
		PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass);
		if (PlayerWidget)
		{
			PlayerWidget->AddToViewport();
		}
	}
	if (EnemyWidgetClass)
	{
		EnemyWidget = CreateWidget<UUserWidget>(GetWorld(), EnemyWidgetClass);
		if (EnemyWidget)
		{
			EnemyWidget->AddToViewport();
		}
	}

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController != nullptr)
	{
		CombatPlayer = Cast<ACombatPlayer>(PlayerController->GetPawn());
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatEnemy::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		ACombatEnemy* Enemy = Cast<ACombatEnemy>(Actor);
		if (Enemy != nullptr)
			CombatEnemyArr.Add(Enemy);
	}
	
	
}

void ACombatGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CombatPlayer == nullptr || CombatEnemyArr.Num() == 0)
		return;

	if (RestartWidget == nullptr || !RestartWidget->IsInViewport())
	{
		// 判断敌人是否全部死亡
		bool bAllEnemyIsDead = true;
		for (ACombatEnemy* Enemy : CombatEnemyArr)
		{
			if (!Enemy->GetCharacterState().bIsDead)
			{
				bAllEnemyIsDead = false;
				break;
			}
		}

		if (CombatPlayer->GetCharacterState().bIsDead || bAllEnemyIsDead)
		{
			// 当Player死亡或者Enemy全部死亡

			// 禁用Player输入
			CombatPlayer->DisableInput(GetWorld()->GetFirstPlayerController());
			
			// 检查是否存在有效绑定
			if (OnGameOverDelegate.IsBound())
			{
				OnGameOverDelegate.Broadcast();
			}

			// 弹出重新开始Widget
			ShowRestartWidget();
		}
	}
	
}

void ACombatGameMode::ShowRestartWidget()
{
	
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	if (RestartWidgetClass)
	{
		RestartWidget = CreateWidget<UUserWidget>(GetWorld(), RestartWidgetClass);
		if (RestartWidget)
		{
			if (PlayerWidget && PlayerWidget->IsInViewport())
				PlayerWidget->RemoveFromViewport();

			if (EnemyWidget && EnemyWidget->IsInViewport())
				EnemyWidget->RemoveFromViewport();

			RestartWidget->AddToViewport();
		}
	}
}
