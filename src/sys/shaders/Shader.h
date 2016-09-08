// Various shader functionality for loading and compiling GLSL shaders

#pragma once
#include "../msys.h"
#ifdef DEBUG
	GLchar *LoadShaderText(const char *fileName);
#endif

// get fragment shader text from resource
const char* shaderFragFromResource(int resourceId); 
// get vertex shader fragment text from resource
const char* shaderVertFromResource(int resourceId);
// init shader object
void initShader( int *pid, const char *vs, const char *fs, const char *shaderName );
// set shader param (1 int value)
void setShaderParam1i(int shaderId, char* paramName, int val);
// set shader param (2 int values)
void setShaderParam2iv(int shaderId, char* paramName, int* vals);
// set shader param (1 float value)
void setShaderParam1f(int shaderId, char* paramName, float val);

void setShaderParam2f(int shaderId, char* paramName, float val, float val2);

// etc etc
void setShaderParam2fv(int shaderId, char* paramName, float* vals);
void setShaderParam3fv(int shaderId, char* paramName, float* vals);
void setShaderParam4fv(int shaderId, char* paramName, float* vals);


