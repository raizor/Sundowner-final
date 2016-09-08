#include "menger.h"


/************************************************************************/
/*  Setup   
/************************************************************************/
menger::menger()
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

	initShader(&_shaderIdFractal, shaderVertFromResource(IDS_SHADER_MENGER_VERT), shaderFragFromResource(IDS_SHADER_MENGER_FRAG), "Fract");
	initShader(&_shaderIdTunnel, NULL, shaderFragFromResource(IDS_SHADER_TUNNEL_FRAG), "Tunnel");
	initShader(&_shaderIdMixer, NULL, shaderFragFromResource(IDS_SHADER_MIXER_FRAG), "Mixer");
}

void menger::HandleMu()
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

extern int SynthNotesPrev[17];
extern int SynthNotes[17];
extern bool SynthNoteHandled[17];

int seed = 0;
static int pid;
static float fparams[4*4];
static float lightPos[3];
static float movement[3];
static float displace1;
static float displace1target;
static float threshold = 0.01f;
static float maxIters = 200;
static float lightStrength = 1;
static float ambient = 0.1f;
static float maxD = 10;

int randInt(int floor, int ceil)
{
	int ret = msys_rand(&seed) % (ceil+1-floor);
	ret+=floor;
	return ret;
}

float mx = 0;
float my = 0;
float tmx = 0;
float tmy = 0;
float xm = 0.1f;
float ym = 0.3f;

int msx = 0;
int msy = 0;

float tAdd = 0;
float iters = 10;
float tStage = 0;

int spongeIterations = 0;
int boxIterations = 0;

float twistX = 0.0f;
float twistY = 0.0f;
float twistZ = 0.0f;

float twistXtarget = 0.0f;
float twistYtarget = 0.0f;

bool spongeUp = true;

/************************************************************************/
/*  Draw                        
/************************************************************************/
void menger::Draw(float time, bool* hitBeatInfo)
{
	// get synth notes

    float t  = 0.001f*(float)time;
	
    // camera position
    fparams[ 0] = 2.0f*sin(1.0f*t+0.1f);
    fparams[ 1] = t;
    fparams[ 2] = 2.0f*cos(1.0f*t);
    // camera target
    fparams[ 4] = 0.0f;
    fparams[ 5] = 0.0f;
    fparams[ 6] = 0.0f;
    // sphere
    fparams[ 8] = 0.0f;
    fparams[ 9] = 0.0f;
    fparams[10] = 0.0f;
    fparams[11] = 1.0f;

	fparams[0] = t;

	mx = (t*xm) - tmx;
	my = (t*ym) - tmy;
	
	movement[0] = mx;
	movement[1] = my;


	if (!SynthNoteHandled[0] && SynthNotes[0] == 48)
	{
		// lead bell - random iteration count
	}

	if (!SynthNoteHandled[0] && SynthNotes[0]!=0)
	{
		if (spongeUp)
		{
			if (spongeIterations < 4)
			{
				spongeIterations++;
			}else{
				spongeUp = false;
			}
		}else{
			if (spongeIterations > 0)
			{
				spongeIterations--;
			}else{
				spongeUp  =true;
			}
		}

	}
	
	if (!SynthNoteHandled[2] && SynthNotes[1] == 55)
	{		
		tAdd+=1.0f;
		tStage+=0.8f;
	}

	// 48 = c4
	// 55 = g4
	// 43 = g3
	// 46 = a#3
	// 53 = f4

	if (!SynthNoteHandled[2] && SynthNotes[2] !=0)
	{		
		// twist x/y
		float ax = (float)randInt(0,1000)/1000.0f;
		float ay = (float)randInt(0,1000)/1000.0f;
				
		if (twistXtarget > 0.0f)
		{
			twistXtarget = ax > 0 ? ax : -ax;
		}else{
			twistXtarget = ax > 0 ? -ax : ax;
		}

		if (twistYtarget > 0.0f)
		{
			twistYtarget = ay > 0 ? ay : -ay;
		}else{
			twistYtarget = ay > 0 ? -ay : ay;
		}
	}

	// cut holes (increase iterations)
	if (!SynthNoteHandled[11] &&(SynthNotes[11] == 60 || SynthNotes[11] == 62 ))
	{
		if (boxIterations < 4)
		{
			boxIterations++;
		}		
		if (spongeIterations < 4)
		{
			spongeIterations++;
		}

		// displace x/y/z
		float ax = (float)randInt(0,1000)/2000.0f;
		displace1target = ax;
	}

	float inc = 0.01f;

	if (twistXtarget > twistX)
	{
		if (twistX+inc > twistXtarget)
		{
			twistX=twistXtarget;
		}else{
			twistX+=inc;
		}
	}else if (twistXtarget < twistX)
	{
		if (twistX-inc < twistXtarget)
		{
			twistX=twistXtarget;
		}else{
			twistX-=inc;
		}
	}else{
		// reached transition end, transition back to 0
		twistXtarget = 0.0f;
	}


	if (twistYtarget > twistY)
	{
		if (twistY+inc > twistYtarget)
		{
			twistY=twistYtarget;
		}else{
			twistY+=inc;
		}
	}else if (twistYtarget < twistY)
	{
		if (twistY-inc < twistYtarget)
		{
			twistY=twistYtarget;
		}else{
			twistY-=inc;
		}
	}else{
		// reached transition end, transition back to 0
		twistYtarget = 0.0f;
	}

	t+=tAdd;

	if (displace1< 0.4f) displace1+=0.001f;

	float res[2];
	res[0] = (float)globalScreenInfo.width;
	res[1] = (float)globalScreenInfo.height;
	glDisable(GL_DEPTH_TEST);
	oglUseProgram( _shaderIdFractal );
	oglUniform1f( oglGetUniformLocation( _shaderIdFractal, "twistX" ), twistX );
	oglUniform1f( oglGetUniformLocation( _shaderIdFractal, "twistY" ), twistY );
	oglUniform1f( oglGetUniformLocation( _shaderIdFractal, "twistZ" ), twistZ );	
    oglUniform4fv( oglGetUniformLocation( _shaderIdFractal, "fpar" ),  4, fparams );
	oglUniform1f( oglGetUniformLocation( _shaderIdFractal, "time" ), time );
	oglUniform2f( oglGetUniformLocation( _shaderIdFractal, "resolution" ), res[0], res[1] );
	oglUniform1i( oglGetUniformLocation( _shaderIdFractal, "boxIterations" ), boxIterations);
	oglUniform1i( oglGetUniformLocation( _shaderIdFractal, "spongeIterations" ), spongeIterations);
	oglUniform1f( oglGetUniformLocation( _shaderIdFractal, "displace1" ),  displace1 );

    glRectf( -1, -1, 1, 1 );
	oglUseProgram( 0 );

	glEnable(GL_DEPTH_TEST);
}

/************************************************************************/
/*  Destructor, we don't care...                          
/************************************************************************/
menger::~menger(void)
{

}
