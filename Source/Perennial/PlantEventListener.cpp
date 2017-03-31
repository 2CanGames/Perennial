// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "PlantActor.h"
#include "PlantEventListener.h"

PlantEventListener::~PlantEventListener()
{
}

void PlantEventListener::processEvent()
{
	MyActor->DayEnded();
}
