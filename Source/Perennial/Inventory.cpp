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

void UInventory::addItemToInventory(AInventoryItem *item) {
	items.Add(item);
}

void UInventory::removeItemToInventory(AInventoryItem *item) {

	for (auto& CurrentItem : items)
	{
		if (CurrentItem == item) {
			items.Remove(CurrentItem);
		}
	}
}

void UInventory::removeItemToInventory(FString plantName) {
	for (int i = 0; i < items.Num(); i++) {
		if (items[i]->getPlantName().Compare(plantName) == 0) {
			items.RemoveAt(i);
			return;
		}
	}
}

int UInventory::getTotalQualityPoints() {

	int totalQualityPoints = 0;

	for (auto& CurrentItem : items)
	{
		totalQualityPoints += CurrentItem->getQuality();
	}

	return totalQualityPoints;
}


