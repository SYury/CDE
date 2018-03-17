#include "player.h"

Player player[MAX_PLAYERS];

int alivePlayers;

Color playerColorById(int id){
	if(id == -1)return grayColor;
	if(id == 0)return redColor;
	if(id == 1)return blueColor;
	return whiteColor;
}
