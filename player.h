#pragma once

#include "color.h"
#include "units.h"

const int MAX_PLAYERS = 8;

struct Player{
	int food = 0, gold = 0;
	int id;
	int bases;
	bool lost = false;
	bool human = false;
	NationFactory * factory;
};

extern Player player[];
extern int alivePlayers;

extern Color playerColorById(int id);
