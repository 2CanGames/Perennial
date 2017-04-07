// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EventListener.h"

class APlantActor;

/**
 * 
 */
class PERENNIAL_API PlantEventListener : AEventListener
{
private:
	APlantActor * MyActor;
public:
	PlantEventListener(APlantActor * actor) : AEventListener() {
		MyActor = actor; 
	}
	void processEvent();
};