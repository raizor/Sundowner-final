#pragma once
#include "../../msys_glext.h"

class GlTexture2d
{
public:
	GLuint id;
	int width, height;
	GlTexture2d(void);
	~GlTexture2d(void);
	void BindTexture(bool bind);

};

