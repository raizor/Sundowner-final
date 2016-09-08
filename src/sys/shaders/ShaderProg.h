#pragma once

#include "../msys.h"

class ShaderProg
{
public:
	char* fragPath;
	char* vertPath;
	char* name;
	GLint vertId;
	GLint fragId;
	GLint progId;
		
	ShaderProg(const char* vertPath, const char* fragPath, const char* shaderName);
	ShaderProg(int vertResourceId, int fragResourceId, const char* shaderName);
	const char* ShaderProg::shaderFromResource(int resourceId, const char* resourceType);
	~ShaderProg(void);
	GLchar* ShaderProg::LoadShaderText(const char *fileName, bool localPath);
	void Refresh();
private:
	void Init(const char* vertData, const char* fragData);
};

