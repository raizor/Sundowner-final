//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//
#include <stdlib.h>
#include "sys/msys.h"
#include "sys/extra/msys_extra.h"
#include "resources/resource.h"
#include "intro.h"
#include "introbits/menger.h"
#include "introbits/julia.h"
#include "introbits/logo.h"
#include "sys/GL/fbo/fboManager.h"

typedef struct
{
    long    mTo;
	int     mSeed;
}IntroObject;

GLfloat fAspect;
static IntroObject intro;
float frames = 0.0f;
int _noMusic;
SCREEN_INFO* screen;

int patternNum = -1;
int prevPatternNum = -1;
int rowNum = -1;
int prevRowNum = -1;
float angle =0;
bool music = false;

extern double timeVal;

GLint _shaderIdPostFx;

GLint _shaderPostFx;
GLint windowWidth = 512;               // window size
GLint windowHeight = 512;
GLint textureWidth = 512;              // texture size
GLint textureHeight = 512;
GLint maxTexSize;                       // maximum allowed size for 1D/2D texture
GLfloat texCoordOffsets[18];

GLuint fb, fbTex, depth_rb, recTex, ptex;

menger *_menger;
julia *_julia;
logo *_logo;

fboManager *_fboMan;
fbo *fbo1024, *fboFullscreen2, *fbo512, *fbo512_2, *fboFullscreen, *fboFullscreen3;
GLuint textureId;

// 4klang sync
extern int SynthNotesPrev[17];
extern int SynthNotes[17];
extern bool SynthNoteHandled[17];

// quit exit anim
extern float quitValX;
extern float quitValY;
extern double quitTime;
extern bool quit;

#define lerpx(t, a, b) ( a + t * (b - a) )

static float deform = 5.0f;
static float flash = 0.0f; // white flash amount for post fx shader
static bool flashed = false; 
static float split = 0.0f;

// lens screen distort
static float distortMod = 1.0f;
static float distort1s = 0.525f;
static float distort2s = 0.525f;
static float distort3s = 0.525f;
static float distort1 = 0.525f;
static float distort2 = 0.525f;
static float distort3 = 0.525f;

extern bool quitting;

bool distortingUp = false;
bool distorting = false;
float distortTime = 0;

float dmodinc = 0.0f;



// timings for intro anim
float introStart = 10.0f;
float part1start = 0+introStart;
float part1end = 1.0f+introStart;
float part2start = 1.0f+introStart;
float part2end = 2.0f+introStart;
float part3start = 2.0f+introStart;
float part3end = 4.0f+introStart;

float fxtime = 0;
float barsPercent = 1.0f;
float fxStartTime = 10.0f;

// what scene is playing?
#define spongeX 0
#define juliaX 1
#define greetsX 2
int showWhat = spongeX;

bool doingTransisition = false;
bool transitionUp = false;
float introTimes[10];
int introStage = 1;

// party-hacked init stuff to fix last minute prob
void setupVars()
{
    introStage = 1;
	deform = 5.0f;
	flash = 0.0f;
	flashed = false;
	split = 0.0f;
	distortMod = 1.0f;

	distort1s = 0.525f;
	distort2s = 0.525f;
	distort3s = 0.525f;

	distort1 = 0.525f;
	distort2 = 0.525f;
	distort3 = 0.525f;

	distortingUp = false;
	distorting = false;
    distortTime = 0;
    dmodinc = 0.0f;
    introStart = 10.0f;
    part1start = 0+introStart;
    part1end = 1.0f+introStart;
    
    part2start = 1.0f+introStart;
    part2end = 2.0f+introStart;
    
    part3start = 2.0f+introStart;
    part3end = 4.0f+introStart;
    
    fxtime = 0;
    barsPercent = 1.0f;
    fxStartTime = 10.0f;
}

// init the main texture (logo) etc
void initTex()
{
	introTimes[0] = 0; // sponge
	introTimes[1] = 56; // julia
	introTimes[2] = 80; // greets 
	introTimes[3] = 108; // sponge 
    
	introTimes[4] = 117; // julia 

	introTimes[5] = 153; // sponge
	introTimes[6] = 10000;
	introTimes[7] = 10000;
	introTimes[8] = 10000;
	introTimes[9] = 10000;

	GLint width, height, icomps;
	GLenum eFormat;
	GLbyte *pixels = gltLoadPNG(IDR_PNG_LOGO, &width, &height, &icomps, &eFormat );
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, icomps, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pixels );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

	free(pixels);
}

// set up global shared FBOs
void initFbo()
{
	_fboMan = globalFboManager;
	fboInfo finfo;
	finfo.name = "1024";
	finfo.numColorBuffers = 1;
	finfo.generateMipMaps = false;
	finfo.internalFormat = GL_RGBA8;
	finfo.format = GL_BGRA;
	finfo.dataType = GL_UNSIGNED_BYTE;
	finfo.width = globalScreenInfo.width;	
	finfo.pixelData = NULL;
	finfo.height = globalScreenInfo.height;	
	finfo.magFilter = GL_LINEAR;
	finfo.minFilter = GL_LINEAR;
	finfo.wrapModeX = GL_CLAMP_TO_EDGE;
	finfo.wrapModeY = GL_CLAMP_TO_EDGE;
	fbo1024 = _fboMan->addFbo(finfo);

	finfo.name = "512";
	finfo.numColorBuffers = 1;
	finfo.width = 512;
	finfo.height = 512;
	finfo.magFilter = GL_LINEAR;
	finfo.minFilter = GL_LINEAR;
	finfo.wrapModeX = GL_MIRRORED_REPEAT;
	finfo.wrapModeY = GL_MIRRORED_REPEAT;
	fbo512 = _fboMan->addFbo(finfo);

	// optimize - reduce size
	finfo.name = "Fullscreen";
	finfo.numColorBuffers = 2;
	finfo.width = globalScreenInfo.width;
	finfo.height =  globalScreenInfo.height;
	fboFullscreen = _fboMan->addFbo(finfo);

	finfo.name = "Fullscreen2";
	fboFullscreen2 = _fboMan->addFbo(finfo);

    finfo.name = "Fullscreen3";
    fboFullscreen3 = _fboMan->addFbo(finfo);

    finfo.name = "512_2";
    finfo.numColorBuffers = 1;
    finfo.width = 512;
    finfo.height = 512;
    finfo.magFilter = GL_LINEAR;
    finfo.minFilter = GL_LINEAR;
    finfo.wrapModeX = GL_MIRRORED_REPEAT;
    finfo.wrapModeY = GL_MIRRORED_REPEAT;
    fbo512_2 = _fboMan->addFbo(finfo);
}

// init everything
int intro_init(int nomusic, IntroProgressDelegate *pd, SCREEN_INFO *screenInfo )
{
	setupVars();

	//_noMusic = true;//nomusic;
    // progress report, (from 0 to 200)
    pd->func( pd->obj, 0 );
	CheckGlError(false);
	screen = screenInfo;

	initFbo();
	CheckGlError(false);
	//MsgBox("msg", "hello");	

	_menger = new menger();	
	_julia = new julia();

	//_scroller = new fwScroller();
	CheckGlError(false);
	_logo = new logo();
	CheckGlError(false);

	initTex();

    // init your stuff here (mzk player, intro, ...)
    // remember to call pd->func() regularly to update the loading bar

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(screenInfo->height == 0)
        screenInfo->height = 1;
	
    glViewport(0, 0, screenInfo->width, screenInfo->height);
        
	glShadeModel( GL_SMOOTH );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor( 0, 0, 0, 0 );

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    gluPerspective(30.0, screenInfo->aspect, 1.0, 10000.0);
    
    pd->func( pd->obj, 50 );

    intro.mTo = msys_timerGet();

	pd->func( pd->obj, 100 );
	CheckGlError(false);
    if (globalScreenInfo.addNoise)
    {
	    initShader(&_shaderPostFx, shaderVertFromResource(IDS_SHADER_TV_VERT), shaderFragFromResource(IDS_SHADER_TV_FRAG), "Fx");
    }else{
        initShader(&_shaderPostFx, shaderVertFromResource(IDS_SHADER_TV_VERT), shaderFragFromResource(IDS_SHADER_TVNN_FRAG), "Fx");
    }
	CheckGlError(false);
	glMatrixMode(GL_MODELVIEW);
	// force initial draw to avoid shader compile lag (ati?)
	bool hitBeatInfo[4] = {false, false, false, false};
	fboFullscreen2->BindFbo(true);
    // todo
	_menger->Draw(0, hitBeatInfo);
	_julia->Draw(0, hitBeatInfo);
	fboFullscreen2->BindFbo(false);
	return 1;
}

void intro_end( void )
{    
    // deallocate your stuff here
}

// post processing 
void DrawPostFx(float time)
{
    float quitSpeed = 0.01f;
	// scene should be in fbo2 texture 1. 
	if (quitting)
	{
        if (quitValY-quitSpeed > 0.005f)
        {
            quitValY-=quitSpeed;
        }else{
            quitValY = 0.005f;
        }
		quitValY -= quitSpeed;
		flash = 1.0f - quitValY;

		if (quitValY < 0.21f)
		{
			quitValX -= quitSpeed;
			if (quitValX < 0.001f) 
			{
				quit = true;
			}
		}
	}
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	fbo *fboFull2 = fboFullscreen2;
	// draw 1024x1024 fbo to screen
	glDisable(GL_DEPTH_TEST);
	oglUseProgram(_shaderPostFx);

	oglActiveTextureARB(GL_TEXTURE0);
	fboFull2->BindTexture(1);	

	float fadeUp = 0.1f;

	if (time > 10.0f - fadeUp && time <= 10.0f)
	{
		flash = (time - (10-fadeUp)) * 20.0f;
	}

	float fadeDown = 0.3f;

	if (time > 10.0f && time <= 10.0f + fadeDown)
	{
		flash = time - 10.0f;
		flash = fadeDown - flash;
		flash = 20.0f * flash;
	}else{
		flash = 0;
	}

	if (flash < 0) flash = 0;

	setShaderParam1i(_shaderPostFx, "screen_w", globalScreenInfo.width);
	setShaderParam1i(_shaderPostFx, "screen_h", globalScreenInfo.height);
	setShaderParam1f(_shaderPostFx, "time", time);
	setShaderParam1f(_shaderPostFx, "value1", flash); // white add for flash fx
	setShaderParam1f(_shaderPostFx, "value2", 100); // color splitting
	setShaderParam1f(_shaderPostFx, "deform_value", deform); // screen glass distortion
	setShaderParam1i(_shaderPostFx, "texture", 0);
	setShaderParam1f(_shaderPostFx, "distort1", distort1*distortMod < 1.0f ? distort1*distortMod : 1.0f);
	setShaderParam1f(_shaderPostFx, "distort2", distort2*distortMod < 1.0f ? distort2*distortMod : 1.0f);
	setShaderParam1f(_shaderPostFx, "distort3", distort3*distortMod < 1.0f ? distort3*distortMod : 1.0f);
	

	glPushMatrix();
	{
		glLoadIdentity();
		glTranslatef(0,0,-1);

		float scaleX = 0.5f;
		float scaleY = 0.33f;

		glBegin(GL_QUADS);
		{
			glTexCoord3f(0, 0, 0);
			glVertex3f(-scaleX*quitValX, -scaleY*quitValY, 0);

			glTexCoord3f(0, 1, 0);
			glVertex3f(-scaleX*quitValX, scaleY*quitValY, 0);

			glTexCoord3f(1, 1, 0);
			glVertex3f(scaleX*quitValX, scaleY*quitValY, 0);

			glTexCoord3f(1, 0, 0);
			glVertex3f(scaleX*quitValX, -scaleY*quitValY, 0);
		}		
		glEnd();
	}
	glPopMatrix();
	oglUseProgram(0);
	fboFull2->BindTexture(0);
	glEnable(GL_DEPTH_TEST);

	float mult = 6.1f;

	if ((float)timeVal*mult < 4.5f)
	{
		deform = 4.5f - ((float)timeVal*mult);
	}else{
		deform = 0.5f;
	}
}

// reload shaders (for debugging)
void intro_reloadShaders()
{
    _julia->refreshShaders();
    _logo->refreshShaders();
}

// draw png logo overaly on main scenes
void DrawLogo()
{
	glPushMatrix();
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glColor4f(1,1,1,1);
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glTranslatef(0,0,-1);
				
		glBindTexture(GL_TEXTURE_2D, textureId);

		float scale = 0.82f;

		float sizeX = 0.7f*scale;
		float sizeY = 0.13f*scale;

		float startX = 0.0f - (sizeX/2.0f);
		float startY = 0.060f;

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0,1);
			glVertex3f(startX, startY, 0);

			glTexCoord2f(0, 0);
			glVertex3f(startX, startY+sizeY, 0);

			glTexCoord2f(1,0);
			glVertex3f(startX+sizeX, startY+sizeY, 0);

			glTexCoord2f(1,1);
			glVertex3f(startX+sizeX, startY, 0);
		}		
		glEnd();


		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}

bool distorting1 = false;
bool distorting2 = false;
bool distorting3 = false;

// draw the black bars
void DrawBars(float timeVal)
{
	float botPos = -20.0f;
	float topPos = 0.11625f;

	glColor4f(1,1,1,1);

	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	{
		glLoadIdentity();
		glTranslatef(0,0,-1);
		float pixelSizeX = 1.0f / globalScreenInfo.width;
		float pixelSizeY = 1.0f / globalScreenInfo.height;
		
		if (timeVal >= part1start && timeVal <= part1end)
		{
			fxtime = (float)(((timeVal-introStart)-(part1start-introStart)) / (part1end-introStart)) * 0.5f;
			// dropping dot
			glColor4f(0,0,0,1);
			glBegin(GL_QUADS);
			glVertex3f(-1,-1,0);
			glVertex3f(-1,1,0);
			glVertex3f( 1,1,0);
			glVertex3f( 1,-1,0);
			glEnd();	

			glColor4f(1,1,1,1);
			glBegin(GL_QUADS);
			glVertex3f(0-pixelSizeX,0.5f-pixelSizeY-fxtime,0);
			glVertex3f(0-pixelSizeX,0.5f+pixelSizeY-fxtime,0);
			glVertex3f(0+pixelSizeX,0.5f+pixelSizeY-fxtime,0);
			glVertex3f(0+pixelSizeX,0.5f-pixelSizeY-fxtime,0);
			glEnd();
		}
		
		if (timeVal >= part2start && timeVal <= part2end)
		{
			fxtime = (float)(((timeVal-introStart)-(part2start-introStart)) / (part2end-introStart));
			glColor4f(0,0,0,1);
			glBegin(GL_QUADS);
			glVertex3f(-1,-1,0);
			glVertex3f(-1,1,0);
			glVertex3f( 1,1,0);
			glVertex3f( 1,-1,0);
			glEnd();	

			// white lines
			glColor4f(1,1,1,1);
			// bottom 
			glBegin(GL_LINES);
			glVertex3f( 0-fxtime,0,0);
			glVertex3f( 0+fxtime,0,0);
			glEnd();
		}

		if (timeVal >= part3start && timeVal <= part3end)
		{
			fxtime = (float)(((timeVal-introStart)-(part3start-introStart)) / (part3end-introStart))*2;

			// black bars
			glColor4f(0,0,0,1);

			float ypBot = lerpx(fxtime, 0, -0.20f);
			float ypTop = lerpx(fxtime, 0, topPos);

			// bottom
			glBegin(GL_QUADS);
			glVertex3f(-1,-1,0);
			glVertex3f(-1,ypBot,0);
			glVertex3f( 1,ypBot,0);
			glVertex3f( 1,-1,0);
			glEnd();

			// top
			glBegin(GL_QUADS);
			glVertex3f(-1,ypTop,0);
			glVertex3f(-1,1,0);
			glVertex3f( 1,1,0);
			glVertex3f( 1,ypTop,0);
			glEnd();
			
			// white lines
			glColor4f(1,1,1,1);

			// bottom 
			glBegin(GL_LINES);
			glVertex3f(-1,ypBot,0);
			glVertex3f( 1,ypBot,0);
			glEnd();

			// top
			glBegin(GL_LINES);
			glVertex3f(-1,ypTop,0);
			glVertex3f( 1,ypTop,0);
			glEnd();
		}
		
		if (timeVal >= part3end)
		{
			// black bars
			glColor4f(0,0,0,1);

			// bottom
			glBegin(GL_QUADS);
			glVertex3f(-1,-1,0);
			glVertex3f(-1,-0.20f*barsPercent,0);
			glVertex3f( 1,-0.20f*barsPercent,0);
			glVertex3f( 1,-1,0);
			glEnd();

			// top
			glBegin(GL_QUADS);
			glVertex3f(-1,topPos*barsPercent,0);
			glVertex3f(-1,1,0);
			glVertex3f( 1,1,0);
			glVertex3f( 1,topPos*barsPercent,0);
			glEnd();

			// white lines
			glColor4f(1,1,1,1);

			glDisable(GL_DEPTH_TEST);

			glBegin(GL_LINES);
			// bottom 
			glVertex3f(-1, -0.20f*barsPercent,0);
			glVertex3f( 1, -0.20f*barsPercent,0);
			// top
			glVertex3f(-1, topPos*barsPercent,0);
			glVertex3f( 1, topPos*barsPercent,0);

			glEnd();
			glEnable(GL_DEPTH_TEST);
		}
	}
	glPopMatrix();
}

// main intro loop
int intro_do( void )
{
    bool hitBeatInfo[4] = {false, false, false, false}; // sync hit?
	float ftime = (float)timeVal;
	if (timeVal >= introTimes[introStage])
	{
		doingTransisition = true;
	}

	if (distort1 > distort1s)
	{
		distort1-=0.001f;
	}

	if (distort2 > distort2s)
	{
		distort2-=0.001f;
	}

	if (distort3 > distort3s)
	{
		distort3-=0.001f;
	}

	if (doingTransisition)
	{
		if (transitionUp)
		{
			// open bars
			float tt = ftime - introTimes[introStage];
			tt*=1.5f;
			barsPercent = (tt < 1.0f ? tt : 1.0f);
			if (barsPercent >= 1.0f)
			{
				// stage done
				doingTransisition = false;
				transitionUp = false;
				introStage++;
			}
		}else{
			// close bars
			float tt = ftime - introTimes[introStage];
			tt*=1.5f;
			barsPercent = 1.0f - (tt < 1.0f ? tt : 1.0f);
			if (barsPercent <= 0.0f)
			{
				transitionUp = true;
				introTimes[introStage] =ftime;
				// swap effect
                if (introStage == 2)
                {
                    showWhat = greetsX;
                }else{
                    if (showWhat == spongeX)
                    {
                        showWhat = juliaX;
                    }else if (showWhat == juliaX){
                        showWhat = spongeX;
                    }else if (showWhat == greetsX){
                        showWhat = juliaX;
                    }
                }
			}
		}

	}

	CheckGlError(false);
	CheckGlError(false);
	glClearColor(0,0,0,0); // red
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glViewport(0, 0, globalScreenInfo.width, globalScreenInfo.height); // change viewport to match screen size
         
	if (timeVal > fxStartTime)
	{
		fboFullscreen2->BindFbo(true);
	}
	if (timeVal <=introStart)
	{
		// cubes logo
		_logo->draw((float)timeVal);
	}else{
        
		if (showWhat == spongeX)
		{
            _menger->Draw((float)timeVal, false); // sponge
		}else if (showWhat == juliaX)
        {
            float jtime = (ftime*2) - 200.0f;
            // julia
            _julia->Draw(jtime, &hitBeatInfo[0]); // julia
		}else if (showWhat == greetsX)
        {
            _logo->drawGreets(ftime - introTimes[2] , &hitBeatInfo[0]);
        }		
	}
	
	fxtime = 0;
    
	DrawBars(ftime);
	
	if (timeVal > 10.0f)
	{
		// png logo
		DrawLogo();		
	}
    	
	if (timeVal > fxStartTime)
	{
		fboFullscreen2->BindFbo(false);
	}
		
	glColor3f(1,1,1);
	if (timeVal > fxStartTime)
	{
		oglUseProgram(_shaderPostFx);
		glPushMatrix();
		{
			fboFullscreen2->BindTexture(1);
			DrawPostFx((float)timeVal);	
		}
		glPopMatrix();
		oglUseProgram(0);
	}
	CheckGlError(false);	
	frames+=1.0f;		

	if (!SynthNoteHandled[1] && SynthNotes[1] == 48)
	{		
		split = 10.0f;
	}

	if ((!SynthNoteHandled[0x0b] && SynthNotes[0x0b] == 2))
	{
		distortingUp = true;
		distorting = true;
		distortTime = ftime;
	}
	
	if (distorting)
	{
		if (distortingUp)
		{
			if (distortMod < 1.06f)
			{
				distortMod+= ((ftime - distortTime)*0.00017f) ;
			}else{
				distortingUp = false;
			}
		}else{
			if (distortMod > 1.0f)
			{
				distortMod -= ((ftime - distortTime)*0.00017f) ;
			}else{
				distorting = false;
			}
		}
	}

	if ((!SynthNoteHandled[0] && SynthNotes[0] == 48)) 
	{
		distorting1 = true;
		dmodinc = 0.0001f;
		distortMod = 1.04f;
		distort1 *= 1.01f;
		distort2 *= 1.02f;
		distort3 *= 1.03f;
	}

	if (!SynthNoteHandled[4] && SynthNotes[4] != 0)
	{
		distorting1 = true;
		dmodinc = 0.0001f;
		distortMod = 1.04f;
		distort1 *= 1.01f;
		distort2 *= 1.01f;
		distort3 *= 1.01f;
	}

	if (!SynthNoteHandled[0x0a] && SynthNotes[0x0a] != 0) // strings screetch
	{
		distorting1 = true;
		dmodinc = 0.0001f;
		distortMod = 1.04f;
		distort1 *= 1.01f;
		distort2 *= 1.02f;
		distort3 *= 1.025f;
	}

	if (distortMod > 1.0f)
	{
		distortMod-=dmodinc;
		dmodinc*=0.999f;
		if (distortMod < 1.0f) distortMod = 1.0f;
	}

	if (distort1 > 0.525f)
	{
		distort1-=dmodinc;
		dmodinc*=0.999f;
	}

	if (distort2 > 0.525f)
	{
		distort2-=dmodinc;
		dmodinc*=0.999f;
	}

	if (distort2 > 0.525f)
	{
		distort2-=dmodinc;
		dmodinc*=0.999f;
	}

	for(int i=0; i<17; i++)
	{
		SynthNoteHandled[i] = true;
	}

	return( 0 );
}