#include "tiles.h"

const Tile grassTile = Tile('#', greenColor, tile_grass, true);
const Tile mountainTile = Tile('^', grayColor, tile_mountain, true);
const Tile sandTile = Tile('#', yellowColor, tile_sand, true);

Tile* tileById(tile_id id){
	if(id == tile_grass)return grassTile.clone();
	if(id == tile_mountain)return mountainTile.clone();
	if(id == tile_sand)return sandTile.clone();
	return nullptr;
}
