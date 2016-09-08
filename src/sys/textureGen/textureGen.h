#pragma once
#include "../msys.h"

class textureGen
{
public:
	textureGen(void);
	GLuint GenerateNoiseTexture();
	~textureGen(void);
};
