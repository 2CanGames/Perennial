// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EventListener.h"

class APlantActor;

/**
 * 
 */
class PERENNIAL_API PlantEventListener : EventListener
{
private:
	APlantActor * MyActor;
public:
	PlantEventListener(APlantActor * actor) : EventListener() {
		MyActor = actor; 
	}
	void processEvent();
};