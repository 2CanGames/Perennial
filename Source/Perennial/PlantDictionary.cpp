// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PlantDictionary.h"


EPlantType APlantDictionary::MapToPlantType(FString type) const
{
	type = type.ToLower();

	if (type == "tree") return (EPlantType::TREE);
	else if (type == "vine") return (EPlantType::VINE);
	else if (type == "root") return (EPlantType::ROOT);
	else if (type == "bush") return (EPlantType::BUSH);

	return EPlantType();
}

// Sets default values
APlantDictionary::APlantDictionary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!PlantLookupTable) {
		static ConstructorHelpers::FObjectFinder<UDataTable> PlantLookupDataTable_BP(TEXT("DataTable'/Game/Data/PlantData.PlantData'"));

		if (PlantLookupDataTable_BP.Object) {
			PlantLookupTable = PlantLookupDataTable_BP.Object;
		}
	}
}

// Called when the game starts or when spawned
void APlantDictionary::BeginPlay()
{
	Super::BeginPlay();
	//Go thru table and plop data into dictionary
	FString ContextString;
	TArray<FName> plantNames;
	plantNames = PlantLookupTable->GetRowNames();

	for (int i = 0; i < plantNames.Num(); i++) {
		FName name = plantNames[i];
		FPlantLookupTable* row = PlantLookupTable->FindRow<FPlantLookupTable>(name, ContextString);
		if (row)
		{
			EPlantType Type = MapToPlantType(row->Plant_Type);
			if (!PlantDictionary.Contains(Type))
				PlantDictionary.Add(Type);
			TArray<FPlant>* typeTable = PlantDictionary.Find(Type);
			FPlant plant;
			plant.Name = name.ToString();
			plant.Quality = row->Quality;
			plant.Type = MapToPlantType(row->Plant_Type);
			typeTable->Add(plant);
		}
	}
}

// Called every frame
void APlantDictionary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString APlantDictionary::GetRandomPlantOfType(EPlantType type)
{
	TArray<FPlant>* Plants = PlantDictionary.Find(type);
	int Random = FMath::RandRange(0, Plants->Num() - 1);
	//TODO: Return a plant based on its quality
	return (*Plants)[Random].Name;
}

