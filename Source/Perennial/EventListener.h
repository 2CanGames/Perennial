// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * An inteface that allows objects to listen to the 
 * TimeController and implement unique behavior
 */
class PERENNIAL_API EventListener
{
public:

	EventListener();

	// process event with derived class behavior
	virtual void processEvent() = 0;
};
