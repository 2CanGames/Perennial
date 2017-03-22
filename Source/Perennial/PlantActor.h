// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "PlantActor.generated.h"

UENUM(BlueprintType)
enum PlantType {
	TREE,
	ROOT,
	VINE
};

UENUM(BlueprintType)
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DayEnded();
		virtual void DayEnded_Implementation();

	//UStaticMeshComponent* CurrentPlantMesh;

	//TArray<UStaticMeshComponent*> PlantMeshes;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsWatered;
	bool bIsFertilized;
	bool bIsHarvestable;

	int Quality;
	int DaysAlive;

	void SetType(FString newType);

	/* Blueprint Accessible Functions */

	UFUNCTION(BlueprintCallable)
		void SetType(PlantType newType);
	
	UFUNCTION(BlueprintCallable)
		PlantType GetType() const;

	UFUNCTION(BlueprintCallable)
		void SetStage(PlantStage newStage);

	UFUNCTION(BlueprintCallable)
		PlantStage GetStage() const;

	UPROPERTY(BlueprintReadOnly, Category = "Plant Data")
		FString PlantName;

	UFUNCTION(BlueprintCallable)
		void InitPlant(FString name);

	UFUNCTION()
		void Grow();

	/* TODO: Change return type to InventoryItem[] when implemented*/
	UFUNCTION()
		void Harvest();
	
};