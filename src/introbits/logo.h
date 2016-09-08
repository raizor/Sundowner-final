#pragma once

#include "../sys/msys.h"
#include "../resources/resource.h"
#include "../sys/extra/msys_extra.h"
#include "../sys/extra/RNG.h"
#include "../sys/shaders/ShaderProg.h"
#include "logo.h"

#include "../sys/textureGen/textureGen.h"

// stuff for drawing the logo made of blocks at start of intro
// ** also draws the greets now
class logoBlock
{
	public:
		vertex targetPos;
		vertex currentPos;
		float ax, ay, az;
		color4f color;
		color4f colorEnd;
		float speed;
		float startTime;
		float percent, percent2;
		float height;
		float rotSpeed;
		float scalex, scaley, scalez;
};

struct CubeSettings {
	float sourcePos[3];
	float targetPos[3];
	float blockCol[3];
	float  percentDone;
};

class logo
{
public:

    void logo::refreshShaders();

	// greets stuff
	GLuint *vboGVerts3, *vboGNormals, *vboGColors, *vboGTexCoords1, *vboGTexCoords2, *vboGTexCoords3;
	vertex **gbn;
	vertex4 **gverts4, **gtc4_1, **gtc4_2, **gtc4_3;
	color4f **gvertCols;
	ShaderProg *_shaderIdGreets, *_shaderIdWater;
    ShaderProg* shaderEdge;
    float *gXpos, *gYpos, *gZpos;
	void logo::makeGreets(GLbyte* pixels);
	int *gnumblocks;
	int numGroups;
	char** groupNames;
    void logo::waterGreets(float time);
    void logo::GDemoLight(float time);
    void logo::GDemoLight2(float time);
    void logo::GDemoLight3(float time);
    void logo::GDrawFboPlasma(float time, bool* hitBeatInfo);

	// logo stuff
	int charWidths[256];
	fbo *fboFullscreen, *fboFullscreen2, *fboFullscreen3, *fbo512_2, *fbo1024;
	vertex *bpTarget, *bpStart, *bpCurrent, *bn, *bnt;
	vertex *angleTarget, *angleCurrent, *angleStart;
	ShaderProg *_shaderIdLogo, *_shaderIdRays;
	GLuint tbuffer;
	GLuint vboVerts3, vboNormals, vboColors, vboTexCoords1, vboTexCoords2, vboTexCoords3;
	int *indices;

	RNG *rng;
	int seed;
	vertex pixelSize;
	float *pct;
	bool *bactive;
	color4f *bc;
	float cubeSize;

	vertex4 *verts4, *tc4_1, *tc4_2, *tc4_3;
	color4f *vertCols;	
	int numblocks;
	static GLuint textureId, lookupTextureId;
	logo(void);
	void logo::makeGroupGreet(int groupNum, GLbyte* pixels);
	void logo::drawGroupGreet(int groupNum, float time);
	void logo::move(float time);
	void draw(double timeIn);
	void logo::DemoLight(float time);
	void logo::DemoLight2(float time);
	void logo::DemoLight3(float time);
	static color4f getBlockColor(GLbyte* pixels, int x, int y);
	int logo::createCube(int idx, int cubeNumVal, color4f color, color4f* vCols, vertex4* v4, vertex* ns, float depth);
	void getBlocks(GLbyte* pixels, GLbyte* destPixels);
	void logo::drawCube(float offsetX, float offsetY, float offsetZ, color4f color);
	void logo::drawGreets(float time, bool* hitBeatInfo);
	~logo(void);
};

