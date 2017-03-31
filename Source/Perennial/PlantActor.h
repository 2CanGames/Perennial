// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "InventoryItem.h"
#include "PlantActor.generated.h"

UENUM(BlueprintType)
enum EPlantType {
	TREE,
	ROOT,
	VINE
};

UENUM(BlueprintType)
enum EPlantStage {
	NO_PLANT,
	SEED,
	BUDDING,
	GROWN
};

class PlantEventListener;

UCLASS()
class PERENNIAL_API APlantActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlantActor();

private:
	PlantEventListener* OnDayEndedListener;

	UDataTable* PlantLookupTable;

	UClass* BP_Plantable;

	int _TimeSinceLastWatering;
	EPlantStage _CurrentStage;
	EPlantType _Type;

	void InitPlant(FString name);

	void Grow();

	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Data")
		TMap<TEnumAsByte<EPlantStage>, USkeletalMesh*> MeshMap;

	UFUNCTION(BlueprintCallable)
		void SetType(EPlantType newType);

	UFUNCTION(BlueprintCallable)
		EPlantType GetType() const;

	UFUNCTION(BlueprintCallable)
		void SetStage(EPlantStage newStage);

	UFUNCTION(BlueprintCallable)
		EPlantStage GetStage() const;

	UPROPERTY(BlueprintReadOnly, Category = "Plant Data")
		FString PlantName;

	UFUNCTION(BlueprintCallable)
		void DayEnded();

	UFUNCTION()
		void Plant(InventoryItem * item);

	UFUNCTION()
		void Water();

	UFUNCTION()
		void Fertilize();

	/* TODO: Change return type to InventoryItem[] when implemented*/
	UFUNCTION()
		void Harvest();
};