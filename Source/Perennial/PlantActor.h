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

	//Listener for when TimeController broadcasts the day ending
	PlantEventListener* OnDayEndedListener;

	//Data lookup table for plant information
	UDataTable* PlantLookupTable;

	//Days since this plant was last watered
	int _TimeSinceLastWatering;

	//The current plant stage this plant is on
	EPlantStage _CurrentStage;

	//The type of plant this plant is currently
	EPlantType _Type;

	//Initialize the plant given a name
	void InitPlant(FString name);

	//Grow the plant to the next stage
	void Grow();

	//Resets the plant back into a plantable area
	void Die();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		class USkeletalMeshComponent* PlantMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Plant")
		int DaysToGrow = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Plant")
		int FertilizerSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Status")
		class UBillboardComponent* WaterIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Status")
		class UParticleSystemComponent* FertilizerEffect;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsWatered;
	bool bIsFertilized;
	bool bIsHarvestable;

	int Quality;
	int DaysAlive;

	void SetType(FString newType);

	void SetIsWatered(bool newBool);

	void SetIsHarvestable(bool newBool);

	void SetIsFertilized(bool newBool);

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
		void Plant(UInventoryItem * item);

	UFUNCTION()
		void Water();

	UFUNCTION()
		void Fertilize();

	/* TODO: Change return type to InventoryItem[] when implemented*/
	UFUNCTION()
		void Harvest();
};