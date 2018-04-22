#pragma once

#include "tiles.h"
#include "other.h"

enum unit_type_id{
	unit_infantry,
	unit_cavalry,
	unit_ranged,
	unit_heavy
};

enum nation_id{
	cnation,
	rnation
};

class Unit{
	protected:
	char icon;
	std::string name;
	std::vector<std::string> description;
	unit_type_id unit_type;
	int fraction;
	int health;
	int move_pts;
	int attack_range;
	int low_dmg, high_dmg;
	int gold_cost, food_cost;
	public:
	Unit(char _icon, std::string _name, std::vector<std::string> _description, unit_type_id _unit_type, int _fraction, int _health, int _move_pts, int _attack_range, int _low_dmg, int _high_dmg, int _gold_cost, int _food_cost):icon(_icon),name(_name),description(_description),unit_type(_unit_type),fraction(_fraction),health(_health),move_pts(_move_pts),attack_range(_attack_range),low_dmg(_low_dmg),high_dmg(_high_dmg),gold_cost(_gold_cost),food_cost(_food_cost){}
	~Unit(){}
	virtual int getTileAttackBonus(tile_id id)const = 0;
	virtual int getTileDefenceBonus(tile_id id)const = 0;
	virtual void restoreMP() = 0;
	std::string getName()const{return name;}
	char getIcon()const{return icon;}
	int getMP()const{return move_pts;}
	int getAR()const{return attack_range;}
	int getFraction()const{return fraction;}
	int getHealth()const{return health;}
	int getLD()const{return low_dmg;}
	int getHD()const{return high_dmg;}
	void addMP(int cnt){move_pts += cnt;}
	void setMP(int val){move_pts = val;}
	void setFraction(int fr){fraction = fr;}
	void addHealth(int cnt){health += cnt;}
	void setHealth(int val){health = val;}
	void payFor()const;
	void onDeath()const;
};

class CInfantry;
class CCavalry;
class CRanged;
class CHeavy;

extern const CInfantry protoCInfantry;
extern const CCavalry protoCCavalry;
extern const CRanged protoCRanged;
extern const CHeavy protoCHeavy;

class RInfantry;
class RCavalry;
class RRanged;
class RHeavy;

extern const RInfantry protoRInfantry;
extern const RCavalry protoRCavalry;
extern const RRanged protoRRanged;
extern const RHeavy protoRHeavy;

class NationFactory{
	public:
	virtual Unit * getInfantry(int fraction) = 0;
	virtual Unit * getCavalry(int fraction) = 0;
	virtual Unit * getRanged(int fraction) = 0;
	virtual Unit * getHeavy(int fraction) = 0;
	Unit * unitById(int fraction, unit_type_id id){
		if(id == unit_infantry)return getInfantry(fraction);
		if(id == unit_cavalry)return getCavalry(fraction);
		if(id == unit_ranged)return getRanged(fraction);
		if(id == unit_heavy)return getHeavy(fraction);
		return nullptr;
	}
};

class CFactory : public NationFactory{
	public:
	virtual Unit * getInfantry(int fraction);
	virtual Unit * getCavalry(int fraction);
	virtual Unit * getRanged(int fraction);
	virtual Unit * getHeavy(int fraction);
};

class RFactory : public NationFactory{
	public:
	virtual Unit * getInfantry(int fraction);
	virtual Unit * getCavalry(int fraction);
	virtual Unit * getRanged(int fraction);
	virtual Unit * getHeavy(int fraction);
};

extern NationFactory * factoryById(nation_id id);
