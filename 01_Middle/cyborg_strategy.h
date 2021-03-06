#pragma once
#include "abstract_strategy.h"
#include "super_player.h"
#include "imgui\imgui.h"

//A human-controlled global strategy, ideal for testing tactics
// Behaviour is similar to real-time strategy games.
class CyborgStrategy : public AbstractStrategy
{
public:
	CyborgStrategy() {}
	~CyborgStrategy() {}

	virtual void changeOrders(AllOrders &newallorders)
	{ //just like startTurn;
		//allorders = newallorders;
		newallorders = allorders;
		endturn = false;
		iterations = 0;
	}

	virtual UNIT_TYPE train()
	{
		return whattotrain;
	}

	virtual bool Update() //return true when the turn is ended
	{
		++iterations;
		return iterations > wait;
	}
	virtual void Render(); //implemented in cpp
public:
	AllOrders allorders;
	bool endturn = false;
	int wait = 1000000;
	int iterations = 0;
	UNIT_TYPE whattotrain = ROCK;
};