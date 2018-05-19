#pragma once

#include "army.h"
#include "buildings.h"

class Defender{
	int dmg;
	tile_id id;
public:
	Defender(int _dmg, tile_id _id):dmg(_dmg), id(_id){}
	bool defend(Army& army)const;
	bool defend(Squad& squad)const;
	bool defend(UnitInSquad& u)const;
	bool defend(Base& base)const;
};
