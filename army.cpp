#include "army.h"

int UISStrong::attack(tile_id where)const{
	int damage = component->attack(where);
	damage += 3;
	return damage;
}

bool UISStrong::defend(int damage, tile_id where){
	return component->defend(damage, where);
}

int UISArmored::attack(tile_id where)const{
	return component->attack(where);
}

bool UISArmored::defend(int damage, tile_id where){
	damage = std::max(damage - 3, 0);
	return component->defend(damage, where);
}

bool cleanSquad(Squad * s, Squad * parent){
	if(s->isUnit())
		return true;
	std::vector<Squad*> children = s->getChildSquads();
	for(auto nxt : children)
		cleanSquad(nxt, s);
	if(s->empty()){
		if(parent != nullptr)
			parent->removeSquad(s);
		return false;
	}
	return true;
}
