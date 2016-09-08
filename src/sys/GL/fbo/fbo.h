#pragma once
#include "../../msys_glext.h"

/*
Fbo properties
*/
typedef struct fboInfo
{
	char* name;
	int numColorBuffers;
	int width, height;
	void* pixelData;
	bool generateMipMaps;
	GLint minFilter, magFilter, wrapModeX, wrapModeY, internalFormat, format, dataType;
}fboInfo;

/*
Fbo object
*/
class fbo
{
private:
	bool _fboCreated;
public:	
	GLuint id, depthTextureId;
	GLuint *textureIds;
	bool fboBound, depthTextureBound;
	int boundTextureNum;
	fboInfo info;
	fbo(fboInfo info);
	~fbo(void);

	bool CreateFbo(GLvoid* pixelData);
	void BindFbo(bool bind);
	void BindTexture(int colorBufferNumber);
	void BindDepthTexture(bool bind);

	
};
