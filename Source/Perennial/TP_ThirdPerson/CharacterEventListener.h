// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EventListener.h"

class ACharacterActor;

/**
 * 
 */
class PERENNIAL_API CharacterEventListener : EventListener
{
private:
	ACharacterActor * MyActor;

public:
	CharacterEventListener(ACharacterActor * actor) : EventListener() 
	{
		MyActor = actor;
	};

	void processEvent();
};
