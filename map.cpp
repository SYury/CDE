#include "map.h"
#include "random.h"
#include "defender.h"

void loadMap(FILE * fl){
	//TODO: this must be rewritten
	if(fl == NULL)die("Attempted to load from NULL");
	Map *map = (new Map)->Instance();
	map->clear();
	int players;
	if(!fscanf(fl, "%d", &players))die("Broken map");
	for(int i = 0; i < MAX_PLAYERS; i++){
		player[i].lost = true;
		if(i < players){
			int x, gold, food, bases, na;
			if(!fscanf(fl, "%d%d%d%d%d", &x, &gold, &food, &bases, &na))die("Broken map");
			player[i].lost = x;
			player[i].gold = gold;
			player[i].food = food;
			player[i].bases = bases;
			player[i].factory = factoryById((nation_id)na);
		}
	}
	if(!fscanf(fl, "%d", &map->H))die("Broken map");
	if(!fscanf(fl, "%d", &map->W))die("Broken map");
	map->resize();
	for(int i = 0; i < map->H; i++){
		for(int j = 0; j < map->W; j++){
			int id;
			if(!fscanf(fl, "%d", &id))die("Broken map");
			map->tiles[i].push_back(*tileById((tile_id)id));
		}
	}
	for(int i = 0; i < map->H; i++){
		for(int j = 0; j < map->W; j++){
			int type;
			int fraction;
			int health, move_pts;
			if(!fscanf(fl, "%d", &type))die("Broken map");
			if(type == -1){map->units[i][j] = nullptr; continue;}
			if(!fscanf(fl, "%d%d%d", &fraction, &health, &move_pts))die("Broken map");
			Unit * who = player[fraction].factory->unitById(fraction, (unit_type_id)type);
			who->setHealth(health);
			who->setMP(move_pts);
			Army * ar = (Army*)(new UnitInSquad(who));;
			map->units[i][j] = ar;
		}
	}
	for(int i = 0; i < map->H; i++){
		for(int j = 0; j < map->W; j++){
			int ex;
			int owner, hp;
			if(!fscanf(fl, "%d", &ex))die("Broken map");
			if(ex == -1){map->bases[i][j] = nullptr; continue;}
			if(!fscanf(fl, "%d%d", &owner, &hp))die("Broken map");
			Base * base = new Base(i, j);
			base->setHp(hp);
			base->setOwner(owner);
			map->bases[i][j] = base;
		}
	}
	for(int i = 0; i < map->H; i++){
		for(int j = 0; j < map->W; j++){
			int ex;
			int owner;
			if(!fscanf(fl, "%d", &ex))die("Broken map");
			if(ex == -1){map->villages[i][j] = nullptr; continue;}
			if(!fscanf(fl, "%d", &owner))die("Broken map");
			Village * vil = new Village(i, j);
			vil->setOwner(owner);
			map->villages[i][j] = vil;
		}
	}
	for(int i = 0; i < map->H; i++){
		for(int j = 0; j < map->W; j++){
			int ex;
			int owner;
			if(!fscanf(fl, "%d", &ex))die("Broken map");
			if(ex == -1){map->towns[i][j] = nullptr; continue;}
			if(!fscanf(fl, "%d", &owner))die("Broken map");
			Town * town = new Town(i, j);
			town->setOwner(owner);
			map->towns[i][j] = town;
		}
	}
}

void saveMap(FILE * fl){
	//TODO
	if(fl == NULL)die("Attempted to save into NULL");
	Map *map = (new Map)->Instance();
}

void moveArmy(Army * who, int dx, int dy){
	Map *map = (new Map)->Instance();
	int x = who->getX() + dx, y = who->getY() + dy;
	if(who->getMP() == 0)return;
	if(!map->inMap(x, y))die("Army tried to move out of the map!");
	if(map->units[x][y] != nullptr)return;
	if(map->bases[x][y] != nullptr)return;
	if(map->villages[x][y] != nullptr)map->villages[x][y]->capture(who->getFraction());
	if(map->towns[x][y] != nullptr)map->towns[x][y]->capture(who->getFraction());
	map->units[who->getX()][who->getY()] = nullptr;
	who->addX(dx);
	who->addY(dy);
	who->addMP(-1);
	map->units[who->getX()][who->getY()] = who;
}

void attackArmy(Army * atk, Army * def){
	Map *map = (new Map)->Instance();
	if(atk->getFraction() == def->getFraction())
		return;
	int dmg = atk->attack(map->tiles[atk->getX()][atk->getY()].getId());
	if(!Defender(dmg, map->tiles[def->getX()][def->getY()].getId()).defend(*def)){
		map->units[def->getX()][def->getY()] = nullptr;
		delete def;
	}
}

void attackBase(Army * atk, Base * def){
	Map *map = (new Map)->Instance();
	if(atk->getFraction() == def->getOwner())
		return;
	auto dbonus = 80;
	int dmg = atk->attack(map->tiles[atk->getX()][atk->getY()].getId());
	double raw_dmg = dmg/double(dbonus);
	dmg = (int)(round(raw_dmg) + 1e-8);
	if(!Defender(dmg, map->tiles[def->getX()][def->getY()].getId()).defend(*def)){
		map->bases[def->getX()][def->getY()] = nullptr;
		delete def;
	}
}

bool canUpgrade(int x, int y){
	Map *map = (new Map)->Instance();
	int fr = map->units[x][y]->getFraction();
	for(int i = x - 3; i <= x + 3; i++){
		for(int j = y - 3; j <= y + 3; j++){
			if(!map->inMap(i, j))continue;
			if(map->units[i][j] != nullptr && map->units[i][j]->getFraction() != fr)
				return false;
		}
	}
	return true;
}
