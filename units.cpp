#include "units.h"
#include "player.h"

void Unit::payFor()const{player[fraction].food -= food_cost; player[fraction].gold -= gold_cost;}
void Unit::onDeath()const{player[fraction].food += food_cost;}

class CInfantry : public Unit{
	public:
	CInfantry(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 60;
		if(id == tile_sand)return 70;
		if(id == tile_mountain)return 40;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id);
	}
	void restoreMP(){move_pts = 2;}
	CInfantry * clone(int _fraction = -1)const{return new CInfantry(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class CCavalry : public Unit{
	public:
	CCavalry(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 70;
		if(id == tile_sand)return 60;
		if(id == tile_mountain)return 30;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id);
	}
	void restoreMP(){move_pts = 3;}
	CCavalry * clone(int _fraction = -1)const{return new CCavalry(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class CRanged : public Unit{
	public:
	CRanged(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 50;
		if(id == tile_sand)return 50;
		if(id == tile_mountain)return 50;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		if(id == tile_grass || id == tile_sand)return 50;
		if(id == tile_mountain)return 60;
		return 0;
	}
	void restoreMP(){move_pts = 2;}
	CRanged * clone(int _fraction = -1)const{return new CRanged(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class CHeavy : public Unit{
	public:
	CHeavy(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 60;
		if(id == tile_sand)return 50;
		if(id == tile_mountain)return 40;
		return 0;
	}
	void restoreMP(){move_pts = 1;}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id) + 10;
	}
	CHeavy * clone(int _fraction = -1)const{return new CHeavy(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class RInfantry : public Unit{
	public:
	RInfantry(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 70;
		if(id == tile_sand)return 60;
		if(id == tile_mountain)return 40;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id);
	}
	void restoreMP(){move_pts = 2;}
	RInfantry * clone(int _fraction = -1)const{return new RInfantry(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class RCavalry : public Unit{
	public:
	RCavalry(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 80;
		if(id == tile_sand)return 50;
		if(id == tile_mountain)return 40;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id);
	}
	void restoreMP(){move_pts = 3;}
	RCavalry * clone(int _fraction = -1)const{return new RCavalry(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class RRanged : public Unit{
	public:
	RRanged(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 50;
		if(id == tile_sand)return 50;
		if(id == tile_mountain)return 50;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id);
	}
	void restoreMP(){move_pts = 2;}
	RRanged * clone(int _fraction = -1)const{return new RRanged(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

class RHeavy : public Unit{
	public:
	RHeavy(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):Unit(_icon, _name, _description, _unit_type, _fraction, _health, _move_pts, _attack_range, _low_dmg, _high_dmg, _gold_cost, _food_cost){}
	int getTileAttackBonus(tile_id id)const{
		if(id == tile_grass)return 70;
		if(id == tile_sand)return 50;
		if(id == tile_mountain)return 40;
		return 0;
	}
	int getTileDefenceBonus(tile_id id)const{
		return getTileAttackBonus(id) + 5;
	}
	void restoreMP(){move_pts = 1;}
	RHeavy * clone(int _fraction = -1)const{return new RHeavy(icon, name, description, unit_type, _fraction, health, move_pts, attack_range, low_dmg, high_dmg, gold_cost, food_cost);}
};

const CInfantry protoCInfantry('S', "Sacred Band of Carthage", {}, unit_infantry, -1, 30, 2, 1, 5, 10, 20, 1);
const CCavalry protoCCavalry('n', "Numidian Cavalry", {}, unit_cavalry, -1, 25, 3, 1, 6, 12, 25, 1);
const CRanged protoCRanged('s', "Balearic Slinger", {}, unit_ranged, -1, 20, 2, 2, 5, 10, 25, 1);
const CHeavy protoCHeavy('E', "Battle Elephant", {}, unit_heavy, -1, 45, 1, 1, 10, 15, 40, 2);

const RInfantry protoRInfantry('i', "Legion Infantry", {}, unit_infantry, -1, 32, 2, 1, 5, 9, 20, 1);
const RCavalry protoRCavalry('c', "Legion Cavalry", {}, unit_cavalry, -1, 27, 3, 1, 5, 11, 25, 1);
const RRanged protoRRanged('b', "Legion Bowman", {}, unit_ranged, -1, 23, 2, 2, 4, 10, 25, 1);
const RHeavy protoRHeavy('C', "Catapult", {}, unit_heavy, -1, 40, 1, 1, 12, 17, 40, 2);

Unit * CFactory::getInfantry(int fraction){Unit * res = protoCInfantry.clone(); res->setFraction(fraction); return res;}
Unit * CFactory::getCavalry(int fraction){Unit * res = protoCCavalry.clone(); res->setFraction(fraction); return res;}
Unit * CFactory::getRanged(int fraction){Unit * res = protoCRanged.clone(); res->setFraction(fraction); return res;}
Unit * CFactory::getHeavy(int fraction){Unit * res = protoCHeavy.clone(); res->setFraction(fraction); return res;}

Unit * RFactory::getInfantry(int fraction){Unit * res = protoRInfantry.clone(); res->setFraction(fraction); return res;}
Unit * RFactory::getCavalry(int fraction){Unit * res = protoRCavalry.clone(); res->setFraction(fraction); return res;}
Unit * RFactory::getRanged(int fraction){Unit * res = protoRRanged.clone(); res->setFraction(fraction); return res;}
Unit * RFactory::getHeavy(int fraction){Unit * res = protoRHeavy.clone(); res->setFraction(fraction); return res;}

NationFactory * factoryById(nation_id id){
	if(id == cnation)return new CFactory;
	if(id == rnation)return new RFactory;
	return nullptr;
}
