// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlantLookupTable.h"

#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"

UCLASS()
class PERENNIAL_API AInventoryItem : public AActor
{
	GENERATED_BODY()
protected:
	UDataTable* PlantLookupTable;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Name")
	FString plantName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Is Seed")
	bool isSeed;

	int getQuality();

	FString getPlantName();
	void setPlantName(FString plantName);

	bool getIsSeed();
	void setIsSeed(bool isSeed);
public:	
	// Sets default values for this actor's properties
	AInventoryItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
