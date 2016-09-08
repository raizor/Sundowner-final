#pragma once
#include "fbo.h"

class fboManager
{
public:
	fbo** fbos;
	int fboCount;
	int fboIndex;

	fbo* addFbo(fboInfo info);

	fboManager();
	~fboManager(void);
};
