// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Inventory.h"
#include "PlantActor.h"
#include "EventListener.h"
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
class PERENNIAL_API ACharacterActor : public AEventListener
{
	GENERATED_BODY()
	
private:	
	
	// Should have the functions below to be a true singleton, but they cause compilation errors??
	// ACharacterActor(ACharacterActor const&);
	// void operator=(ACharacterActor const&);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:

	// Sets default values for this actor's properties
	ACharacterActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void processEvent();

	void DayEnded();

	bool DeleteFertilizer();

	void PlantSeed(APlantActor* CurrentPlant, UInventoryItem* Seed);

	void Harvest(APlantActor* CurrentPlant);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Inventory")
	UInventory* PlayerInventory;

	ECharacterStage _CurrentStage;

	int NumFertilizers;

	int DaysAlive;
	
};
