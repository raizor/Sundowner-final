#include "GlTexture2d.h"


GlTexture2d::GlTexture2d(void)
{
}


GlTexture2d::~GlTexture2d(void)
{
}

void GlTexture2d::BindTexture(bool bind)
{
	glBindTexture(GL_TEXTURE_2D, bind ? id : 0);
}
