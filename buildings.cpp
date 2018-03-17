#include "buildings.h"
#include "map.h"

Unit * Base::hireUnit(Unit * who, int dx, int dy)const{
	Map *map = (new Map)->Instance();
	if(dx == 0 && dy == 0){
		if(hireUnit(who, -1, 0) != nullptr)return who;
		if(hireUnit(who, 1, 0) != nullptr)return who;
		if(hireUnit(who, 0, -1) != nullptr)return who;
		if(hireUnit(who, 0, 1) != nullptr)return who;
	}
	if(!map->inMap(x + dx, y + dy))return nullptr;
	if(map->units[x + dx][y + dy] != nullptr || map->bases[x + dx][y + dy] != nullptr)return nullptr;
	map->units[x + dx][y + dy] = who;
	who->setFraction(belongsTo);
	who->setX(x + dx);
	who->setY(y + dy);
	who->payFor();
	return who;
}
