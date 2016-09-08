#pragma once

#include "../sys/msys.h"
#include "../resources/resource.h"
#include "../sys/extra/msys_extra.h"
#include "../sys/extra/RNG.h"
#include "../sys/shaders/ShaderProg.h"
#include "../sys/textureGen/textureGen.h"

class julia
{
private:

	float rotOffsets[4];
    
	RNG* rng;
	float *muVals;
	int muIndex, muMax;

	int UniformMaxIterations;
	int UniformWidth;
	int UniformHeight;
	int UniformAntiAliasing;
	float UniformAmbientLight;
	float UniformAmbientOcclusion;
	float UniformShadows;
	float UniformSpecularity;
	float UniformSpecularExponent;
	float UniformColorSpread;
	int UniformNumSteps;
	float UniformEpsilon;
	float UniformDelta;
	float UniformEscapeThreshold;
	float UniformBoundingRadius;
	float UniformIntersectMultZ;
	float morphSpeed;

	bool detailUp;

	float UniformBackgroundColor[4];
	float UniformSurfaceColor[4];
	float UniformMu[4];
	float UniformMuTarget[4];
	float muTarget[4];
	float UniformCamera[4];
	float UniformLight[3];
	float UniformLight2[3];
	float UniformLightColor[4];
	float UniformLightColor2[4];

	float mixer;
	float mixerStart;
public:
    
    fbo *fbo512, *fboFullscreen, *fbo1024, *fboFullscreen2;
    static GLuint textureId, noiseTexture;
    static ShaderProg *_shaderIdFractal, *_shaderIdPlasma, *_shaderIdTunnel, *_shaderIdMixer;
	julia();
    void julia::refreshShaders();
	void HandleMu();
	void DrawTunnelToFbo(float time, bool* hitBeatInfo);
	void DrawToScreen(float time, bool* hitBeatInfo);
	void DrawJuliaToFbo(float time, bool* hitBeatInfo);
	void Draw(float time, bool* hitBeatInfo);
	void DrawFboPlasma(float time);
	~julia(void);
};
