// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"

#include "UW_Player.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UUW_Player : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct()override;

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UProgressBar* ProgressBarHealth;
};
