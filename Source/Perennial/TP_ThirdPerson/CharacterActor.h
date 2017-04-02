// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory.h"
#include "GameFramework/Actor.h"
#include "CharacterActor.generated.h"

UENUM(BlueprintType)
enum ECharacterStage
{
	YOUNG,
	PREGNANT,
	OLD
};

UCLASS()
class PERENNIAL_API ACharacterActor : public AActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	ACharacterActor();

	UInventory* PlayerInventory;

	ECharacterStage _CurrentStage;
	
	// Should have the functions below to be a true singleton, but they cause compilation errors??
	// ACharacterActor(ACharacterActor const&);
	// void operator=(ACharacterActor const&);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static ACharacterActor &GetInstance();

	int DaysAlive;

	
	
};
