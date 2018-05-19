#include "defender.h"

bool Defender::defend(Army& army)const{
	return army.defend(dmg, id);
}

bool Defender::defend(Squad& squad)const{
	return squad.defend(dmg, id);
}

bool Defender::defend(UnitInSquad & u)const{
	return u.defend(dmg, id);
}

bool Defender::defend(Base& base)const{
	return base.defend(dmg, id);
}
