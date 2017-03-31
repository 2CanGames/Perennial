// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>

#include "InventoryItem.h"

/**
 * 
 */
class PERENNIAL_API Inventory
{
private:
	std::vector<InventoryItem*> items;

public:
	Inventory();
	~Inventory();

	void addItemToInventory(InventoryItem *item);
	
	void removeItemToInventory(InventoryItem *item);

	int getTotalQualityPoints();
};
