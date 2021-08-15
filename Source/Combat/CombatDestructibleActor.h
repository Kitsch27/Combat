// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatDestructibleActor.generated.h"

UCLASS()
class COMBAT_API ACombatDestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatDestructibleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// UPROPERTY()
	// UDestructibleComponent* MainDestructibleComponent;
	//
	// UPROPERTY()
	// UDestructibleComponent* OtherDestructibleComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGetHit(FVector GetHitLocation, FVector ImpulseDir);

};
