#pragma once

#include "color.h"

enum tile_id{
	tile_grass,
	tile_mountain,
	tile_sand
};

class Tile{
	char icon;
	Color col;
	tile_id id;
	bool passable;
	public:
	Tile(char _icon, Color _col, tile_id _id, bool _passable):icon(_icon),col(_col),id(_id),passable(_passable){}
	Tile* clone()const{return new Tile(icon, col, id, passable);}
	char getIcon()const{return icon;}
	Color getColor()const{return col;}
	tile_id getId()const{return id;}
};

extern const Tile grassTile;
extern const Tile mountainTile;
extern const Tile sandTile;

extern Tile* tileById(tile_id id);
