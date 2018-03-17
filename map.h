#pragma once

#include "other.h"
#include "tiles.h"
#include "units.h"
#include "buildings.h"

class Map{
	static Map* _instance;
	public:
	int W, H;
	std::vector<std::vector<Tile> > tiles;
	std::vector<std::vector<Unit*> > units;
	std::vector<std::vector<Base*> > bases;
	std::vector<std::vector<Village*> > villages;
	std::vector<std::vector<Town*> > towns;
	static void init(){_instance = nullptr;}
	static Map* Instance(){
		if(_instance == nullptr)_instance = new Map;
		return _instance;
	}
	bool inMap(int x, int y){
		return (x >= 0) && (x < H) && (y >= 0) && (y < W);
	}
	void clear(){
		tiles.clear();
		units.clear();
		bases.clear();
		villages.clear();
		towns.clear();
	}
	void resize(){
		tiles.resize(H);
		units.resize(H);
		for(int i = 0; i < W; i++)units[i].resize(W);
		bases.resize(H);
		for(int i = 0; i < W; i++)bases[i].resize(W);
		villages.resize(H);
		for(int i = 0; i < W; i++)villages[i].resize(W);
		towns.resize(H);
		for(int i = 0; i < W; i++)towns[i].resize(W);
	}
};

extern void loadMap(FILE * fl);
extern void saveMap(FILE * fl);

extern void moveUnit(Unit* who, int dx, int dy);
extern void attackUnit(Unit * atk, Unit * def);
extern void attackBase(Unit * atk, Base * def);
