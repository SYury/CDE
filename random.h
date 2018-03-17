#pragma once

#include "other.h"

class Random{
	std::mt19937 base;
	public:
	Random(int seed){base = std::mt19937(seed);}
	Random(){base = std::mt19937(time(0));}
	int get(){return abs(base());}
	int get(int l, int r){
		if(l > r)die("Invalid random range");
		int x = get();
		x %= (r - l + 1);
		return x + l;
	}
};
