#pragma once
#include "GlTexture2d.h"

class TextureManager
{
public:
	GlTexture2d* textures;
	TextureManager(void);
	~TextureManager(void);
};

