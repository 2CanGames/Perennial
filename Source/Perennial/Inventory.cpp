// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "Inventory.h"

UInventory::UInventory()
{
}

UInventory::~UInventory()
{
	items.clear();
}

void UInventory::addItemToInventory(UInventoryItem *item) {
	items.push_back(item);
}

void UInventory::removeItemToInventory(UInventoryItem *item) {

	std::vector<UInventoryItem*>::iterator itr;
	for (itr = items.begin(); itr < items.end(); ++itr)
	{
		if ((*itr) == item) {
			items.erase(itr);
		}
	}
}

int UInventory::getTotalQualityPoints() {

	int totalQualityPoints = 0;

	std::vector<UInventoryItem*>::iterator itr;
	for (itr = items.begin(); itr < items.end(); ++itr)
	{
		totalQualityPoints += (*itr)->getQuality();
	}

	return totalQualityPoints;
}


