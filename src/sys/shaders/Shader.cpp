#include "Shader.h"

// leave this defined to show shader compilation warnings, probably a good idea in debug mode.
// in release mode, warnings and errors are never shown...
#if DEBUG
	#define xSHADER_MSG_ALWAYS
#endif

const char* shaderFragFromResource(int resourceId)
{
	HRSRC		rec;
	HGLOBAL		handle;

	LPCSTR lpResourceId = MAKEINTRESOURCE(resourceId);

	rec = FindResource(NULL, lpResourceId, "FSHADER");
	handle = LoadResource(NULL, rec);

	int size = SizeofResource(NULL, rec);

	const char* data = ( const char*)LockResource(handle);
	char* buffer = new char[size+1];
	msys_memcpy(buffer, data, size);
	buffer[size] = 0; // NULL terminator
	return buffer;
}

const char* shaderVertFromResource(int resourceId)
{
	HRSRC		rec;
	HGLOBAL		handle;

	LPCSTR lpResourceId = MAKEINTRESOURCE(resourceId);

	rec = FindResource(NULL, lpResourceId, "VSHADER");
	handle = LoadResource(NULL, rec);

	int size = SizeofResource(NULL, rec);

	const char* data = ( const char*)LockResource(handle);
	char* buffer = new char[size+1];
	msys_memcpy(buffer, data, size);
	buffer[size] = '\0'; // NULL terminator
	return buffer;
}

void initShader( int *pid, const char *vs, const char *fs, const char *shaderName )
{
#ifdef DEBUG
	msys_debugPrintf("INIT SHADER: %s\n", shaderName);
#endif
    pid[0] = oglCreateProgram();                           
	int vsId;
	int fsId;
	if (vs != NULL)
	{
		vsId = oglCreateShader( GL_VERTEX_SHADER ); 
		oglShaderSource( vsId, 1, &vs, 0 );
		oglCompileShader( vsId );
		oglAttachShader( pid[0], vsId );
	}
	if (fs != NULL)
	{
		fsId = oglCreateShader( GL_FRAGMENT_SHADER ); 
		oglShaderSource( fsId, 1, &fs, 0 );
		oglCompileShader( fsId );
		oglAttachShader( pid[0], fsId );
	}	
	oglLinkProgram( pid[0] );

    #ifdef DEBUG
        int		result;
        char    info[1536];
		char    msgTitle[2000];
        if (vs != NULL)
		{
			oglGetObjectParameteriv( vsId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); 
			oglGetInfoLog( vsId,   1024, NULL, (char *)info ); 
			if( !result )
			{			
				msys_debugPrintf("VERTEX SHADER FAILED: %s\n", info);
				if (strlen(info) > 0)
				{
					sprintf(msgTitle, "VETEX SHADER - %s", shaderName);
					MsgBox(msgTitle, info);
				}
				//MessageBox( 0, info, 0, MB_OK|MB_ICONERROR);
				//DebugBreak();
			}else{
#ifdef SHADER_MSG_ALWAYS
				if (strlen(info) > 0)
				{
					sprintf(msgTitle, "VERTEX SHADER - %s", shaderName);
					MsgBox(msgTitle, info);
				}
#endif
			}
		}
        if (fs != NULL) 
		{
			oglGetObjectParameteriv( fsId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); 
			oglGetInfoLog( fsId,   1024, NULL, (char *)info );
			if( !result )
			{
				msys_debugPrintf("FRAG SHADER FAILED: %s\n", info);
				if (strlen(info) > 0)
				{
					sprintf(msgTitle, "FRAG SHADER - %s", shaderName);
					MsgBox(msgTitle, info);
				}
				DebugBreak();
			}else{
#ifdef SHADER_MSG_ALWAYS
				if (strlen(info) > 0)
				{
					sprintf(msgTitle, "FRAG SHADER - %s", shaderName);
					MsgBox(msgTitle, info);
				}
#endif
			}
		}
        oglGetObjectParameteriv( pid[0], GL_OBJECT_LINK_STATUS_ARB,    &result ); oglGetInfoLog( pid[0], 1024, NULL, (char*)info ); 
		if( !result ) 
		{
			msys_debugPrintf("LINK SHADER FAILED: %s\n", info);
			if (strlen(info) > 0)
			{
				sprintf(msgTitle, "LINK SHADER - %s", shaderName);
				MsgBox(msgTitle, info);
			}
			//DebugBreak();
		}else{
#ifdef SHADER_MSG_ALWAYS
			if (strlen(info) > 0)
			{
				sprintf(msgTitle, "LINK SHADER - %s", shaderName);
				MsgBox(msgTitle, info);
			}
#endif
		}
	#else
		int		result;
		char    info[1536];
		if (vs != NULL)
		{
			oglGetObjectParameteriv( vsId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); 
			oglGetInfoLog( vsId,   1024, NULL, (char *)info ); 
			if( !result )
			{			
				//msys_debugPrintf("VERTEX SHADER FAILED: %s\n", info);
				MessageBox( 0, info, 0, MB_OK|MB_ICONERROR);
				//DebugBreak();
			}
		}
		if (fs != NULL) 
		{
			oglGetObjectParameteriv( fsId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); 
			oglGetInfoLog( fsId,   1024, NULL, (char *)info );
			if( !result )
			{
				//msys_debugPrintf("FRAG SHADER FAILED: %s\n", info);
				MessageBox( 0, info, 0, MB_OK|MB_ICONERROR);
				//DebugBreak();
			}
		}
		oglGetObjectParameteriv( pid[0], GL_OBJECT_LINK_STATUS_ARB,    &result ); oglGetInfoLog( pid[0], 1024, NULL, (char*)info ); 
		if( !result ) 
		{
			//msys_debugPrintf("LINK SHADER FAILED: %s\n", info);
			MessageBox( 0, info, 0, MB_OK|MB_ICONERROR);
			//DebugBreak();
		}	
    #endif
}

#ifdef DEBUG
// Load shader from disk into a null-terminated string
GLchar *LoadShaderText(const char *fileName)
{
	char path[MAX_PATH];
	char shaderPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);

	//sprintf(&shaderPath[0], "%s\\%s", path, fileName);
	sprintf(&shaderPath[0], "%s", fileName);

    GLchar *shaderText = NULL;
    GLint shaderLength = 0;
    FILE *fp;

    fp = fopen(shaderPath, "r");
    if (fp != NULL)
    {
        while (fgetc(fp) != EOF)
        {
            shaderLength++;
        }
        rewind(fp);
        shaderText = (GLchar *)msys_mallocAlloc(shaderLength+1);
        if (shaderText != NULL)
        {
            fread(shaderText, 1, shaderLength, fp);
		}else{

		}
        shaderText[shaderLength] = '\0';
        fclose(fp);
    }

    return shaderText;
}

#endif


void setShaderParam1i(int shaderId, char* paramName, int val)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform1i(uniformLoc, val);
	}
}

void setShaderParam2iv(int shaderId, char* paramName, int* vals)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform2i(uniformLoc, vals[0], vals[1]);
	}
}

void setShaderParam1f(int shaderId, char* paramName, float val)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform1f(uniformLoc, val);
	}
}

void setShaderParam2f(int shaderId, char* paramName, float val, float val2)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform2f(uniformLoc, val, val2);
	}
}

void setShaderParam2fv(int shaderId, char* paramName, float* vals)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform2f(uniformLoc, vals[0], vals[1]);
	}
}

void setShaderParam3fv(int shaderId, char* paramName, float* vals)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform3f(uniformLoc, vals[0], vals[1], vals[2]);
	}
}

void setShaderParam4fv(int shaderId, char* paramName, float* vals)
{
	GLint uniformLoc = oglGetUniformLocation(shaderId, paramName);
	if (uniformLoc != -1)
	{
		oglUniform4f(uniformLoc, vals[0], vals[1], vals[2], vals[3]);
	}
}

