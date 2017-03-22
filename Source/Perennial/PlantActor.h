// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlantActor.generated.h"

UENUM()
enum PlantType {
	FRUIT,
	ROOT,
	VINE
};

UENUM()
enum PlantStage {
	SEED,
	BUDDING,
	GROWN
};

UCLASS()
class PERENNIAL_API APlantActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantActor();

private:
	UDataTable* PlantLookupTable;

	int _TimeSinceLastWatering;
	PlantStage _CurrentStage;
	PlantType _Type;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDayEndHandler();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void InitPlant(FString name, PlantType type);

	UFUNCTION()
		void Grow();

	bool bIsWatered;
	bool bIsFertilized;
	bool bIsHarvestable;

	int Quality;
	int DaysAlive;

	void SetType(PlantType newStage);
	PlantType GetType();

	void SetStage(PlantStage newStage);
	PlantStage GetStage();

	/* TODO: Change return type to InventoryItem[] when implemented*/
	UFUNCTION()
		void Harvest();
	
};
