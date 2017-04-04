// Fill out your copyright notice in the Description page of Project Settings.

#include "Perennial.h"
#include "CharacterEventListener.h"
#include "CharacterActor.h"

void CharacterEventListener::processEvent()
{
	MyActor->DayEnded();
}

