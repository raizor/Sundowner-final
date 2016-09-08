#include "fboManager.h"

/*
	FBO management
*/

fboManager::fboManager()
{
	fboIndex = 0;
	this->fboCount = 10;
	fbos = new fbo*[this->fboCount];
}

fbo* fboManager::addFbo(fboInfo info)
{
	if (fboIndex == fboCount-1) return NULL; // no free FBO slots

	fbo* newFbo = new fbo(info);
	fbos[fboIndex] = newFbo;
	fboIndex++;
	return newFbo;
}

fboManager::~fboManager(void)
{
}
