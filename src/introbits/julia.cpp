#include "julia.h"

extern int SynthNotesPrev[17];
extern int SynthNotes[17];
extern bool SynthNoteHandled[17];


ShaderProg* julia::_shaderIdFractal =0;
ShaderProg* julia::_shaderIdPlasma =0;
ShaderProg* julia::_shaderIdTunnel =0;
ShaderProg* julia::_shaderIdMixer =0;
GLuint julia::textureId = 0;
GLuint julia::noiseTexture = 0;

/************************************************************************/
/*  Setup   
/************************************************************************/
julia::julia()
{

	fbo1024 = globalFboManager->fbos[0];	
	fbo512 = globalFboManager->fbos[1];
	fboFullscreen= globalFboManager->fbos[2];
	fboFullscreen2 = globalFboManager->fbos[3];
	//initTextures();
	rotOffsets[0] = 0.0f;
	rotOffsets[1] = 0.0f;
	rotOffsets[2] = 0.0f;
	rotOffsets[3] = 0.0f;

	mixer = 1.0f;

	rng = new RNG();

	textureGen *tn = new textureGen();
	noiseTexture = tn->GenerateNoiseTexture();

	muIndex = 0;
	muMax = 3;
	muVals = new float[4*3];

	int mi = 0;

	muVals[mi++] = 0.1498f;
	muVals[mi++] = -0.116f;
	muVals[mi++] = -0.157f;
	muVals[mi++] = -0.2441f;

	muVals[mi++] = 0.1498f;
	muVals[mi++] = -0.116f;
	muVals[mi++] = -0.157f;
	muVals[mi++] = -0.2441f;

	muVals[mi++] = -0.03481992f;
	muVals[mi++] = -0.776936f;
	muVals[mi++] = -0.157f;
	muVals[mi++] = -0.2441f;

	UniformMaxIterations = 8;
	UniformWidth = 512;
	UniformHeight = 512;
	UniformAntiAliasing = 0;
	UniformAmbientLight = 0.4f;
	UniformAmbientOcclusion = 1.404f;
	UniformShadows = 0;
	UniformSpecularity = 0.14712f;
	UniformSpecularExponent = 15.93f;
	UniformColorSpread = 0.1f;
	UniformNumSteps = 150;
	UniformEpsilon = 0.001f;
	UniformDelta = 0.0001f;
	UniformEscapeThreshold = 10.0f;
	UniformBoundingRadius = 5.0f;
	UniformIntersectMultZ = 3.0f; // 2.5

	detailUp = false;

	UniformBackgroundColor[0] = 0.0f; UniformBackgroundColor[1] = 0.0f; UniformBackgroundColor[2] = 0.0f; UniformBackgroundColor[3] = 0.0f;
	UniformSurfaceColor[0] = 1.0f; UniformSurfaceColor[1] =1.0f; UniformSurfaceColor[2] = 1.0f; UniformSurfaceColor[3] = 1.0f;
	UniformMu[0] = 0.1498f; UniformMu[1] = -0.1127f; UniformMu[2] = 0.657f; UniformMu[3] = -0.2441f;
	UniformCamera[0] = 1.0f; UniformCamera[1] = 0.0f; UniformCamera[2] = 0.0f; UniformCamera[3] = -3.0f;
	UniformLight[0] = 0.0f; UniformLight[1] = 0.0f; UniformLight[2] = 3.0f;
	UniformLight2[0] = 0; UniformLight2[1] = 0.0f; UniformLight2[2] = 3.0f;

	UniformLightColor[0] = 1.0f; UniformLightColor[1] = 1.0f; UniformLightColor[2] = 0.0f; UniformLightColor[3] = 1.0f;
	UniformLightColor2[0] = 1.0f; UniformLightColor2[1] = 0.0f; UniformLightColor2[2] = 1.0f; UniformLightColor2[3] = 1.0f;
	
	UniformMu[0] = muVals[0];
	UniformMu[1] = muVals[1];
	UniformMu[2] = muVals[2];
	UniformMu[3] = muVals[3];

	morphSpeed = 0.001f;

#if DEBUG
    _shaderIdFractal = new ShaderProg("src\\resources\\shaders\\fract_vs.glsl", "src\\resources\\shaders\\fract_fs.glsl", "FRAG");
    _shaderIdPlasma = new ShaderProg("src\\resources\\shaders\\fract_vs.glsl", "src\\resources\\shaders\\plasma_fs.glsl", "plasma");
    _shaderIdMixer = new ShaderProg("src\\resources\\shaders\\fract_vs.glsl", "src\\resources\\shaders\\mixer_fs.glsl", "mixer");
    _shaderIdTunnel = new ShaderProg("src\\resources\\shaders\\fract_vs.glsl", "src\\resources\\shaders\\rtunnel_fs.glsl", "tunnel");
#else
    _shaderIdFractal = new ShaderProg(IDS_SHADER_FRACTAL_VERT, IDS_SHADER_FRACTAL_FRAG, "FRAG");
    _shaderIdPlasma = new ShaderProg(IDS_SHADER_FRACTAL_VERT, IDS_SHADER_PLASMA_FRAG, "plasma");
    _shaderIdMixer = new ShaderProg(IDS_SHADER_FRACTAL_VERT, IDS_SHADER_MIXER_FRAG, "mixer");
    _shaderIdTunnel = new ShaderProg(IDS_SHADER_FRACTAL_VERT, IDS_SHADER_TUNNEL_FRAG, "Tunnel");
#endif
}

#define testtexj

/************************************************************************/
/*  Create plasma texture from perlin noise in fbo 512x512 (#1)                          
/************************************************************************/
void julia::DrawFboPlasma(float time)
{
#ifndef testtex
	fbo512->BindFbo(true);
#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);

#ifndef testtex
	oglUseProgram(_shaderIdPlasma->progId);
#endif
	float res[2] = {(float)fbo512->info.width, (float)fbo512->info.height};

	setShaderParam2fv(_shaderIdPlasma->progId, "resolution", &res[0]);
	setShaderParam1f(_shaderIdPlasma->progId, "time", time);
	setShaderParam1f(_shaderIdPlasma->progId, "deform_value", (time+10)*0.1f);

	glColor3f(1,1,1);
	glPushMatrix();
	{
		ViewOrtho(fbo512->info.width, fbo512->info.height);		

		glViewport(0, 0, fbo512->info.width, fbo512->info.height); // change viewport to match screen size

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0,1);
			glVertex2i(0,0);

			glTexCoord2f(0,0);
			glVertex2i(0,fbo512->info.height);

			glTexCoord2f(1,0);
			glVertex2i(fbo512->info.width, fbo512->info.height);

			glTexCoord2f(1,1);
			glVertex2i(fbo512->info.width, 0);
		}		
		glEnd();
		ViewPerspective();
		glViewport(0, 0, fboFullscreen2->info.width, fboFullscreen2->info.height); // change viewport to match screen size
	}
	glPopMatrix();
#ifndef testtex
	oglUseProgram(0);

	fbo512->BindFbo(false);
#endif
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

/************************************************************************/
/*  Draw plasma tunnel to 1024x1024 fbo                            
/************************************************************************/
void julia::DrawTunnelToFbo(float time, bool* hitBeatInfo)
{    
	// draw tunnel to 1024x1024 fbo
	fbo1024->BindFbo(true);
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		oglUseProgram(_shaderIdTunnel->progId);
		{
			oglActiveTextureARB(GL_TEXTURE0);
			fbo512->BindTexture(1);

			oglActiveTextureARB(GL_TEXTURE1);
			fboFullscreen->BindTexture(1);

			oglActiveTextureARB(GL_TEXTURE2);
			fboFullscreen->BindTexture(2);
											
			float res[2] = {(float)fbo1024->info.width, (float)fbo1024->info.height};
            
			setShaderParam2fv(_shaderIdTunnel->progId, "size", &res[0]);
			setShaderParam1i(_shaderIdTunnel->progId, "texture", 0);
			setShaderParam1i(_shaderIdTunnel->progId, "texture1f", 1);
			setShaderParam1i(_shaderIdTunnel->progId, "texture2f", 2);
			setShaderParam1f(_shaderIdTunnel->progId, "time", time);
			setShaderParam1f(_shaderIdTunnel->progId, "mixer", mixer);
			setShaderParam1f(_shaderIdTunnel->progId, "aspect", globalScreenInfo.aspect);
			setShaderParam1f(_shaderIdTunnel->progId, "deform_value", time);
            
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

			glColor3f(1,1,1);
			glPushMatrix();
			{
				ViewOrtho(fbo1024->info.width, fbo1024->info.height);		

				glViewport(0, 0, fbo1024->info.width, fbo1024->info.height); // change viewport to match screen size

				glBegin(GL_QUADS);
				{
					glTexCoord2f(0,1);
					glVertex2i(0,0);

					glTexCoord2f(0,0);
					glVertex2i(0,fbo1024->info.height);

					glTexCoord2f(1,0);
					glVertex2i(fbo1024->info.width, fbo1024->info.height);

					glTexCoord2f(1,1);
					glVertex2i(fbo1024->info.width, 0);
				}		
				glEnd();

				glViewport(0, 0, fboFullscreen2->info.width, fboFullscreen2->info.height); // change viewport to match screen size
				ViewPerspective();
			}
			glPopMatrix();

			oglActiveTextureARB(GL_TEXTURE2);
			fboFullscreen->BindTexture(0);

			oglActiveTextureARB(GL_TEXTURE1);
			fboFullscreen->BindTexture(0);

			oglActiveTextureARB(GL_TEXTURE0);
			fbo512->BindTexture(0);
		}
		oglUseProgram(0);

	}
	fbo1024->BindFbo(false);
    
}

// shape morph
void julia::HandleMu()
{
	if (UniformMu[0] == UniformMuTarget[0] && UniformMu[1] == UniformMuTarget[1] && UniformMu[2] == UniformMuTarget[2] && UniformMu[3] == UniformMuTarget[3] )
	{
		// morph to julia with random detail iteration level
		float bound = 1.5f;
		for(int i=0; i<4; i++)
		{
			UniformMuTarget[i] = (rng->rFloat()*bound) - (rng->rFloat()*bound);
		}

	}else{
		for(int i=0; i<4; i++)
		{
			float tval = UniformMuTarget[i];
			if (UniformMu[i] < tval)
			{
				if (UniformMu[i] + morphSpeed <= tval)
				{
					UniformMu[i] += morphSpeed;
				}else{
					UniformMu[i] = tval;
				}	

			}else if (UniformMu[i] > tval)
			{
				if (UniformMu[i] - morphSpeed >= tval)
				{
					UniformMu[i] -= morphSpeed;
				}else{
					UniformMu[i] = tval;
				}
			}
		}
	}
}

/************************************************************************/
/*  Draw julia fractal to 512x512 fbo (#2)                             
/************************************************************************/
void julia::DrawJuliaToFbo(float time, bool* hitBeatInfo)
{	
	fboFullscreen->BindFbo(true);
	{
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT  };
		oglDrawBuffers(2, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		oglUseProgram(_shaderIdFractal->progId);

		float res[2] = {(float)fboFullscreen->info.width, (float)fboFullscreen->info.height};
		setShaderParam2fv(_shaderIdFractal->progId, "resolution", &res[0]);
		setShaderParam1f(_shaderIdFractal->progId, "time", time+rotOffsets[0]);
		setShaderParam1f(_shaderIdFractal->progId, "aspect", globalScreenInfo.aspect);
		setShaderParam1f(_shaderIdFractal->progId, "deform_value", time);

		setShaderParam1i(_shaderIdFractal->progId, "maxIterations", UniformMaxIterations);	
		setShaderParam1i(_shaderIdFractal->progId, "width", UniformWidth);
		setShaderParam1i(_shaderIdFractal->progId, "height", UniformHeight);
		setShaderParam1i(_shaderIdFractal->progId, "texture", 0);
		setShaderParam1i(_shaderIdFractal->progId, "antialiasing", UniformAntiAliasing);
		setShaderParam1i(_shaderIdFractal->progId, "numSteps", UniformNumSteps);
	
		setShaderParam1f(_shaderIdFractal->progId, "ambientLight", UniformAmbientLight);
		setShaderParam1f(_shaderIdFractal->progId, "ambientOcclusion", UniformAmbientOcclusion);
		setShaderParam1f(_shaderIdFractal->progId, "shadows", UniformShadows);
	
		setShaderParam1f(_shaderIdFractal->progId, "specularity", UniformSpecularity);
		setShaderParam1f(_shaderIdFractal->progId, "specularExponent", UniformSpecularExponent);
		setShaderParam1f(_shaderIdFractal->progId, "colorSpread", UniformColorSpread);	
		setShaderParam1f(_shaderIdFractal->progId, "epsilon", UniformEpsilon);
		setShaderParam1f(_shaderIdFractal->progId, "delta", UniformDelta);
		setShaderParam1f(_shaderIdFractal->progId, "escapeThreshold", UniformEscapeThreshold);
		setShaderParam1f(_shaderIdFractal->progId, "boundingRadius", UniformBoundingRadius);
		setShaderParam1f(_shaderIdFractal->progId, "intersectMultZ", UniformIntersectMultZ);

		setShaderParam4fv(_shaderIdFractal->progId, "backgroundColor", &UniformBackgroundColor[0]);	
		setShaderParam4fv(_shaderIdFractal->progId, "surfaceColor", &UniformSurfaceColor[0]);
		setShaderParam3fv(_shaderIdFractal->progId, "light", &UniformLight[0]);
		setShaderParam3fv(_shaderIdFractal->progId, "light2", &UniformLight2[0]);
		setShaderParam4fv(_shaderIdFractal->progId, "lightColor", &UniformLightColor[0]);
		setShaderParam4fv(_shaderIdFractal->progId, "lightColor2", &UniformLightColor2[0]);

		setShaderParam4fv(_shaderIdFractal->progId, "mu", &UniformMu[0]);

		UniformCamera[0] = cos(4.0f+time)+sin(cos(3.0f+time));
		UniformCamera[1] = cos(3.0f+time)+cos(sin(4.0f+time));
		UniformCamera[3] = -(6+(cos(6.0f+time+rotOffsets[0] )+cos(sin(8.0f+time+rotOffsets[0] ))));
		UniformCamera[3] +=mixer*0.75f;

		setShaderParam4fv(_shaderIdFractal->progId, "camera", &UniformCamera[0]);
	
		HandleMu();
		
		oglActiveTextureARB(GL_TEXTURE0);
		fbo1024->BindTexture(1);
	
		glColor3f(1,1,1);

		glPushMatrix();
		{
			ViewOrtho(fboFullscreen->info.width, fboFullscreen->info.height);		
            
			glBegin(GL_QUADS);
			{
				glTexCoord2f(0,1);
				glVertex2i(0,0);

				glTexCoord2f(0,0);
				glVertex2i(0,fboFullscreen->info.height);

				glTexCoord2f(1,0);
				glVertex2i(fboFullscreen->info.width, fboFullscreen->info.height);

				glTexCoord2f(1,1);
				glVertex2i(fboFullscreen->info.width, 0);
			}		
			glEnd();
			ViewPerspective();
		}
		glPopMatrix();
				
		fbo1024->BindTexture(0);

		oglUseProgram(0);
	}
	fboFullscreen->BindFbo(false);

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
}

void julia::refreshShaders()
{
    _shaderIdFractal->Refresh();
    _shaderIdMixer->Refresh();
    _shaderIdPlasma->Refresh();
    _shaderIdTunnel->Refresh();
}

/************************************************************************/
/*  Merge fbos and draw to screen                           
/************************************************************************/
void julia::DrawToScreen(float time, bool* hitBeatInfo)
{	
	// draw 1024x1024 fbo to screen
	oglActiveTextureARB(GL_TEXTURE0);
	fbo1024->BindTexture(1);

	oglActiveTextureARB(GL_TEXTURE1);
	fboFullscreen->BindTexture(1);

	oglActiveTextureARB(GL_TEXTURE2);
	fboFullscreen->BindTexture(2);
		
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	glColor4f(1,1,1, 1);
	oglUseProgram(_shaderIdMixer->progId);

	float res[2];
	res[0] = (float)fboFullscreen2->info.width;
	res[1] = (float)fboFullscreen2->info.height;
	setShaderParam2fv(_shaderIdMixer->progId, "sizeBg", &res[0]);

	res[0] = (float)fboFullscreen->info.width;
	res[1] = (float)fboFullscreen->info.height;
	setShaderParam2fv(_shaderIdMixer->progId, "sizeFract", &res[0]);
	
	setShaderParam1f(_shaderIdMixer->progId, "time", time);
	setShaderParam1f(_shaderIdMixer->progId, "aspect", globalScreenInfo.aspect);
	setShaderParam1f(_shaderIdMixer->progId, "deform_value", (time+10)*0.1f);
	setShaderParam1i(_shaderIdMixer->progId, "textureTunnel", 0);
	setShaderParam1i(_shaderIdMixer->progId, "textureFractalPhong", 1);
	setShaderParam1i(_shaderIdMixer->progId, "textureFractalEnv", 2);
	glPushMatrix();
	{
		ViewOrtho(fboFullscreen2->info.width, fboFullscreen2->info.height);		

		glViewport(0, 0, fboFullscreen2->info.width, fboFullscreen2->info.height); // change viewport to match screen size

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0,0);
			glVertex2i(0,0);

			glTexCoord2f(0,1);
			glVertex2i(0,fboFullscreen2->info.height);

			glTexCoord2f(1,1);
			glVertex2i(fboFullscreen2->info.width, fboFullscreen2->info.height);

			glTexCoord2f(1,0);
			glVertex2i(fboFullscreen2->info.width, 0);
		}		
		glEnd();
		ViewPerspective();
	}
	glPopMatrix();
	oglUseProgram(0);

	oglActiveTextureARB(GL_TEXTURE2);
	fbo1024->BindTexture(0);

	oglActiveTextureARB(GL_TEXTURE1);
	fboFullscreen->BindTexture(0);

	oglActiveTextureARB(GL_TEXTURE0);
	fboFullscreen->BindTexture(0);
	
}

/************************************************************************/
/*  Draw                        
/************************************************************************/
void julia::Draw(float time, bool* hitBeatInfo)
{
	glDisable(GL_DEPTH_TEST);

	if (!SynthNoteHandled[2])
	{	
		hitBeatInfo[0] = true;
	}

	glPushMatrix();
	{	
		if (hitBeatInfo[0] == true)
		{
			mixerStart = time;		
			mixer = 4.0f;
		}

		float mixlen = 2.0f;
        	
		if (mixer > 1.0f)
		{
			mixer-=0.05f;
		}

		UniformCamera[3] = -3 * mixer;
        
		if (hitBeatInfo[1] == true)
		{		
			if (detailUp)
			{
				if (UniformMaxIterations<4)
				{
					UniformMaxIterations++;
				}else{
					detailUp = false;
					UniformMaxIterations--;
				}
			}else{
				if (UniformMaxIterations>1)
				{
					UniformMaxIterations--;
				}else{
					detailUp = true;
					UniformMaxIterations++;
				}
			}
		}

        setShaderParam1f(_shaderIdPlasma->progId, "mode", 1);
		DrawFboPlasma(time * 1);
		DrawTunnelToFbo(time, hitBeatInfo);
		DrawJuliaToFbo(time, hitBeatInfo);
        fboFullscreen2->BindFbo(true);
		DrawToScreen(time, hitBeatInfo);
	}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	
}

/************************************************************************/
/*  Destructor, we don't care...                          
/************************************************************************/
julia::~julia(void)
{
}
