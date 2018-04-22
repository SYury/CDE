#pragma once

#include "units.h"
#include "random.h"

class Army;
class UnitInSquad;

class Squad{
	friend class Army;
	protected:
	int x_;
	int y_;
	std::vector<Squad*> parts_;
	public:
	Squad(){}
	Squad(Squad * who){
		parts_ = {who};
	}
	virtual ~Squad(){std::vector<Squad*>().swap(parts_);}
	virtual int attack(tile_id where)const{
		int damage = 0;
		for(auto s : parts_){
			damage += s->attack(where);
		}
		return damage;
	}
	virtual bool defend(int damage, tile_id where){
		std::vector<Squad*> to_remove;
		for(auto s : parts_){
			bool result = s->defend(damage, where);
			if(!result)
				to_remove.push_back(s);
		}
		while(!to_remove.empty()){
			auto who = to_remove.back();
			to_remove.pop_back();
			auto pos = std::find(parts_.begin(), parts_.end(), who);
			parts_.erase(pos);
			delete who;
		}
		return !parts_.empty();
	}
	bool empty()const{
		return parts_.empty();
	}
	virtual bool isUnit()const{
		return false;
	}
	int getX()const{
		return x_;
	}
	int getY()const{
		return y_;
	}
	void addX(int cnt){
		x_ += cnt;
	}
	void addY(int cnt){
		y_ += cnt;
	}
	void setX(int val){
		x_ = val;
	}
	void setY(int val){
		y_ = val;
	}
	virtual int getMP()const{
		int res = 10000;
		for(auto s : parts_)
			res = std::min(res, s->getMP());
		return res;
	}
	virtual void addMP(int delta){
		for(auto s : parts_)
			s->addMP(delta);
	}
	virtual void restoreMP(){
		for(auto s : parts_)
			s->restoreMP();
	}
	virtual int getFraction()const{
		return parts_[0]->getFraction();
	}
	virtual int getAR()const{
		int res = 10000;
		for(auto s : parts_)
			res = std::min(res, s->getAR());
		return res;
	}
	std::vector<Squad*>::const_iterator begin()const{
		return parts_.cbegin();
	}
	std::vector<Squad*>::const_iterator end()const{
		return parts_.cend();
	}
	void addSquad(Squad * s){
		parts_.push_back(s);
	}
	void removeSquad(Squad * s){
		auto pos = std::find(parts_.begin(), parts_.end(), s);
		parts_.erase(pos);
	}
	std::vector<Squad*> getChildSquads()const{
		return parts_;
	}
};

class UnitInSquad : public Squad{
	Unit * who;
	public:
	UnitInSquad(){}
	UnitInSquad(Unit * _who):who(_who){}
	virtual int attack(tile_id where)const{
		Random gen;
		int dmg = gen.get(who->getLD(), who->getHD());
		return dmg * who->getTileAttackBonus(where);
	}
	virtual bool defend(int damage, tile_id where){
		int dmg = int(damage/double(who->getTileDefenceBonus(where)) + 1e-8);
		who->addHealth(-dmg);
		if(who->getHealth() <= 0){
			who->onDeath();
			delete who;
			return false;
		}
		return true;
	}
	bool isUnit()const{
		return true;
	}
	virtual int getMP()const{
		return who->getMP();
	}
	virtual void addMP(int delta){
		who->addMP(delta);
	}
	virtual void restoreMP(){
		who->restoreMP();
	}
	virtual int getFraction()const{
		return who->getFraction();
	}
	virtual int getAR()const{
		return who->getAR();
	}
	virtual int getHealth()const{
		return who->getHealth();
	}
	virtual std::string getName()const{
		return who->getName();
	}
};

class UISStrong : public UnitInSquad{
	UnitInSquad * component;
	public:
	UISStrong(UnitInSquad * base):component(base){}
	int attack(tile_id where)const;
	bool defend(int damage, tile_id where);
	virtual int getMP()const{
		return component->getMP();
	}
	virtual void addMP(int delta){
		component->addMP(delta);
	}
	virtual void restoreMP(){
		component->restoreMP();
	}
	virtual int getFraction()const{
		return component->getFraction();
	}
	virtual int getAR()const{
		return component->getAR();
	}
	virtual int getHealth()const{
		return component->getHealth();
	}
	virtual std::string getName()const{
		return component->getName();
	}
};

class UISArmored : public UnitInSquad{
	UnitInSquad * component;
	public:
	UISArmored(UnitInSquad * base):component(base){}
	int attack(tile_id where)const;
	bool defend(int damage, tile_id where);
	virtual int getMP()const{
		return component->getMP();
	}
	virtual void addMP(int delta){
		component->addMP(delta);
	}
	virtual void restoreMP(){
		component->restoreMP();
	}
	virtual int getFraction()const{
		return component->getFraction();
	}
	virtual int getAR()const{
		return component->getAR();
	}
	virtual int getHealth()const{
		return component->getHealth();
	}
	virtual std::string getName()const{
		return component->getName();
	}
};

class Army : public Squad{
	public:
	Army(){}
	Army(UnitInSquad * who){
		parts_ = {who};
	}
	template<class T>
	Squad * promote(Squad * what){
		if(what->isUnit()){
			what = dynamic_cast<UnitInSquad*>(new T(dynamic_cast<UnitInSquad*>(what)));
		}
		else{
			for(auto & s : what->parts_)
				s = promote<T>(s);
		}
		return what;
	}
	template<class T>
	void promote(){
		for(auto & s : parts_)
			s = promote<T>(s);
	}
};

extern bool cleanSquad(Squad * s, Squad * parent);
