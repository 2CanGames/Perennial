// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
	items.clear();
}

void Inventory::addItemToInventory(InventoryItem *item) {
	items.push_back(item);
}

void Inventory::removeItemToInventory(InventoryItem *item) {
	
	std::vector<InventoryItem*>::iterator itr;
	for (itr = items.begin(); itr < items.end(); ++itr)
	{
		if ((*itr) == item) {
			items.erase(itr);
		}
	}
}

int Inventory::getTotalQualityPoints() {
	
	int totalQualityPoints = 0;

	std::vector<InventoryItem*>::iterator itr;
	for (itr = items.begin(); itr < items.end(); ++itr)
	{
		totalQualityPoints += (*itr)->getQuality();
	}

	return totalQualityPoints;
}