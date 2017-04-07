// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EventListener.h"

class ACharacterActor;

/**
 * 
 */
class PERENNIAL_API CharacterEventListener : AEventListener
{
private:
	ACharacterActor * MyActor;

public:
	CharacterEventListener(ACharacterActor * actor) : AEventListener()
	{
		MyActor = actor;
	};

	void processEvent();
};
