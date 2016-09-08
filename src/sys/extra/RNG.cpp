#include "../msys_random.h"
#include "RNG.h"

RNG::RNG(void)
{
	_seed = 0;
}

RNG::RNG(int seed)
{
	_seed = seed;
}


int RNG::rInt(int floor, int ceil)
{
	int ret = msys_rand(&_seed) % (ceil+1-floor);
	ret+=floor;
	return ret;
}

float RNG::rFloat()
{
	float ret = msys_frand(&_seed);
	return ret;
}


RNG::~RNG(void)
{
}
