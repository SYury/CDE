#include "buildings.h"
#include "map.h"
#include "other.h"

Army * Base::hireUnit(Unit * who, int dx, int dy)const{
	Map *map = (new Map)->Instance();
	if(dx == 0 && dy == 0){
		Army * res = nullptr;
		if((res = hireUnit(who, -1, 0)) != nullptr)return res;
		if((res = hireUnit(who, 1, 0)) != nullptr)return res;
		if((res = hireUnit(who, 0, -1)) != nullptr)return res;
		if((res = hireUnit(who, 0, 1)) != nullptr)return res;
		return nullptr;
	}
	if(!map->inMap(x + dx, y + dy))return nullptr;
	if(map->units[x + dx][y + dy] != nullptr){
		Army * res = map->units[x + dx][y + dy];
		who->setFraction(belongsTo);
		who->payFor();
		res->addSquad((Squad*)(new UnitInSquad(who)));
		return res;
	}
	if(map->bases[x + dx][y + dy] != nullptr)return nullptr;
	who->setFraction(belongsTo);
	who->payFor();
	Army * res = new Army(new UnitInSquad(who));
	map->units[x + dx][y + dy] = res;
	res->setX(x + dx);
	res->setY(y + dy);
	return res;
}
