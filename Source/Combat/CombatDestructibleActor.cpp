// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatDestructibleActor.h"
#include "DestructibleComponent.h"

// Sets default values
ACombatDestructibleActor::ACombatDestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);
	
	// MainDestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("MainDestructibleComponent"));
	// MainDestructibleComponent->SetupAttachment(Scene);
	//
	// OtherDestructibleComponent = CreateDefaultSubobject<UDestructibleComponent>(TEXT("OtherDestructibleComponent"));
	// OtherDestructibleComponent->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void ACombatDestructibleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACombatDestructibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

