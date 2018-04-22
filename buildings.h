#pragma once

#include "player.h"
#include "army.h"

class Building{
	protected:
	int x, y, belongsTo = -1;
	public:
	Building(int _x, int _y):x(_x),y(_y){}
	Building(int _x, int _y, int _who):x(_x),y(_y),belongsTo(_who){}
	int getX()const{return x;}
	int getY()const{return y;}
	int getOwner()const{return belongsTo;}
	void setX(int _x){x = _x;}
	void setY(int _y){y = _y;}
	void setOwner(int who){belongsTo = who;}
	virtual void capture(int who){belongsTo = who;}
	virtual char getIcon()const = 0;
};

class Base : public Building{
	int hp = 300;
	public:
	Base(int _x, int _y):Building(_x, _y){}
	~Base(){}
	int getHp()const{return hp;}
	void addHp(int cnt){hp += cnt;}
	void setHp(int val){hp = val;}
	char getIcon()const{return 'B';}
	Army * hireUnit(Unit * who, int dx, int dy)const;
};

class Village : public Building{
	int add_food = 3;
	public:
	Village(int _x, int _y):Building(_x, _y){}
	void capture(int who){
		if(belongsTo != -1)player[belongsTo].food -= add_food;
		belongsTo = who;
		if(belongsTo != -1)player[belongsTo].food += add_food;
	}
	char getIcon()const{return 'V';}
};

class Town : public Building{
	int gold_income = 5;
	public:
	Town(int _x, int _y):Building(_x, _y){}
	void makeIncome(){
		if(belongsTo != -1)player[belongsTo].gold += gold_income;
	}
	char getIcon()const{return 'T';}
};
