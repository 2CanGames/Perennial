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

void UInventory::addItemToInventory(UInventoryItem *item) {
	items.Add(item);
}

void UInventory::removeItemToInventory(UInventoryItem *item) {

	for (auto& CurrentItem : items)
	{
		if (CurrentItem == item) {
			items.Remove(CurrentItem);
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


