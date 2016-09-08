#include "fbo.h"

void fbo::BindFbo(bool bind)
{
	oglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, bind ? id : 0);
	fboBound = bind;
}

void fbo::BindDepthTexture(bool bind)
{
	oglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, bind ? depthTextureId : 0);
	depthTextureBound = bind;
}


void fbo::BindTexture(int colorBufferNumber)
{
	glBindTexture(GL_TEXTURE_2D, colorBufferNumber ? textureIds[colorBufferNumber-1] : 0);
}

bool fbo::CreateFbo(GLvoid* pixelData)
{
	fboBound = false;
	boundTextureNum = 0;
	depthTextureBound = 0;
	if (_fboCreated)
	{
		// fbo already exists, delete it (auto frees attached textures)
		oglDeleteFramebuffersEXT(1, &id);
		delete(textureIds);
	}
	textureIds = new GLuint[info.numColorBuffers];
	oglGenFramebuffersEXT(1, &id);
	glGenTextures(info.numColorBuffers, textureIds);
	glGenTextures(1, &depthTextureId);
	
	//switch to our fbo so we can bind stuff to it
	oglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);

	for(int i=0; i<info.numColorBuffers; i++)
	{
		//create the colorbuffer texture and attach it to the frame buffer
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, info.internalFormat, info.width, info.height, 0, info.format, info.dataType, pixelData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, info.wrapModeX);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, info.wrapModeY);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, info.minFilter); // Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, info.magFilter); // Linear Filtering

		// OpenGL will then generate all the required mipmap data for you so that your texture is ready to be used.
		if (info.generateMipMaps)
		{
			oglGenerateMipmapEXT(GL_TEXTURE_2D); 
		}	

		oglFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,	GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, textureIds[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, depthTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, info.width, info.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, info.minFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, info.magFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, info.wrapModeX);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, info.wrapModeY);

	oglFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthTextureId, 0);

	/*
	// create a render buffer as our depth buffer and attach it
	oglBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
	oglRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
		GL_DEPTH_COMPONENT16,globalScreenInfo.fboWidth, globalScreenInfo.fboHeight);

	oglFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,
		GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, depth_rb);
		*/

	// Go back to regular frame buffer rendering
	oglBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	GLenum status = oglCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		_fboCreated = false;
	}else{
		_fboCreated = true;
	}

	return _fboCreated;
}


fbo::fbo(fboInfo info)
{	
	this->info = info;

	CreateFbo(info.pixelData);
}


fbo::~fbo(void)
{
}
