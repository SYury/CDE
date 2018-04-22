#include "map.h"
#include "player.h"

void makeMove(int id){
	//This ai must be replaced
	Map *map = (new Map)->Instance();
	Base *base;
	for(int i = 0; i < map->H; i++)
		for(int j = 0; j < map->W; j++)
			if(map->bases[i][j] != nullptr && map->bases[i][j]->getOwner() == id)base = map->bases[i][j];
	int closest_x = 10000, closest_y = 10000;
	for(int i = 0; i < map->H; i++)
		for(int j = 0; j < map->W; j++)
			if(map->bases[i][j] != nullptr && map->bases[i][j]->getOwner() != id)
				if(abs(base->getX() - closest_x) + abs(base->getY() - closest_y) > abs(base->getX() - i) + abs(base->getY() - j)){closest_x = i; closest_y = j;}
	while(player[id].food >= 1 && player[id].gold >= 25){
		Army * who = base->hireUnit(player[id].factory->getCavalry(id), 0, 0);
		if(who == nullptr)break;
	}
}

void aiTurn(){
	for(int i = 0; i < MAX_PLAYERS; i++){
		if(!player[i].human && !player[i].lost)makeMove(i);
	}
}
