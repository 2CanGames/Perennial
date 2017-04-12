// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "Inventory.h"

UInventory::UInventory()
{
}

UInventory::~UInventory()
{
	items.Empty();
}

void UInventory::addItemToInventory(AInventoryItem *item) 
{
	items.Add(item);
}

void UInventory::removeItemToInventory(AInventoryItem *item) 
{
	int count = 0;
	for (auto& CurrentItem : items)
	{
		if (CurrentItem == item) 
		{
			items.RemoveAt(count);
			return;
		}
		count++;
	}
}

void UInventory::removeItemToInventory(FString plantName) 
{
	int count = 0;
	for (auto& CurrentItem : items) 
	{
		if (CurrentItem->getPlantName().Compare(plantName) == 0) 
		{
			items.RemoveAt(count);
			return;
		}
		count++;
	}
}

int UInventory::getTotalQualityPoints() 
{
	int totalQualityPoints = 0;

	for (auto& CurrentItem : items)
	{
		totalQualityPoints += CurrentItem->getQuality();
	}

	return totalQualityPoints;
}


