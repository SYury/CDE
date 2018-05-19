#pragma once

#include "other.h"

class Random{
	std::mt19937 base;
	public:
	Random(int seed){base = std::mt19937(seed);}
	Random(){base = std::mt19937(time(0));}
	long long get(){return abs((long long)base());}
	int get(int l, int r){
		if(l > r)die("Invalid random range");
		long long x = get();
		x %= (r - l + 1);
		return x + l;
	}
};
