#pragma once

class RNG
{
private:
	int _seed;
public:
	RNG(void);
	RNG(int seed);
	int RNG::rInt(int floor, int ceil);
	float RNG::rFloat();
	~RNG(void);
};
