#include "ShaderProg.h"

#define xSHADER_MSG_ALWAYS

const char* ShaderProg::shaderFromResource(int resourceId, const char* resourceType)
{
	HRSRC		rec;
	HGLOBAL		handle;

	LPCSTR lpResourceId = MAKEINTRESOURCE(resourceId);

	rec = FindResource(NULL, lpResourceId, resourceType);
	handle = LoadResource(NULL, rec);

	int size = SizeofResource(NULL, rec);

	const char* data = ( const char*)LockResource(handle);
	char* buffer = new char[size+1];
	msys_memcpy(buffer, data, size);
	buffer[size] = 0; // NULL terminator
	return buffer;
}

// from disk
ShaderProg::ShaderProg(const char* vertShaderPath, const char* fragShaderPath, const char* shaderName)
{
	vertPath = new char[MAX_PATH];
	fragPath = new char[MAX_PATH];
	name = new char[MAX_PATH];

	sprintf(&vertPath[0], "%s", vertShaderPath);
	sprintf(&fragPath[0], "%s", fragShaderPath);
	sprintf(&name[0], "%s", shaderName);
	
	Init(vertShaderPath == NULL ? NULL : LoadShaderText(vertPath, false), LoadShaderText(fragPath, false));
}

// from resource
ShaderProg::ShaderProg(int vertResourceId, int fragResourceId, const char* shaderName)
{
	vertPath = new char[MAX_PATH];
	fragPath = new char[MAX_PATH];
	name = new char[MAX_PATH];
	sprintf(&vertPath[0], "%s", "");
	sprintf(&fragPath[0], "%s", "");
	sprintf(&name[0], "%s", shaderName);
	Init(vertResourceId == NULL ? NULL :shaderFromResource(vertResourceId, "VSHADER"), shaderFromResource(fragResourceId, "FSHADER"));
}

void ShaderProg::Refresh()
{
	Init(vertPath == NULL ? NULL : LoadShaderText(vertPath, false), LoadShaderText(fragPath, false));
}

void ShaderProg::Init(const char* vertData, const char* fragData)
{
	progId = oglCreateProgram();     
	if (vertData != NULL)
	{
		vertId = oglCreateShader( GL_VERTEX_SHADER ); 
		oglShaderSource( vertId, 1, &vertData, 0 );
		oglCompileShader( vertId );
		oglAttachShader( progId, vertId );
	}
	if (fragData != NULL)
	{
		fragId = oglCreateShader( GL_FRAGMENT_SHADER ); 
		oglShaderSource( fragId, 1, &fragData, 0 );
		oglCompileShader( fragId );
		oglAttachShader( progId, fragId );
	}	
	oglLinkProgram( progId );

#ifdef DEBUG
	int		result;
	char    info[1536];
	char    msgTitle[2000];
	if (vertData != NULL)
	{
		oglGetObjectParameteriv( vertId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); 
		oglGetInfoLog( vertId,   1024, NULL, (char *)info ); 
		if( !result )
		{			
			msys_debugPrintf("VERTEX SHADER FAILED: %s\n", info);
			if (strlen(info) > 0)
			{
				sprintf(msgTitle, "VETEX SHADER - %s", name);
				MsgBox(msgTitle, info);
			}
			//MessageBox( 0, info, 0, MB_OK|MB_ICONERROR);
			//DebugBreak();
		}else{
    #ifdef SHADER_MSG_ALWAYS
			if (strlen(info) > 0)
			{
				sprintf(msgTitle, "VERTEX SHADER - %s", name);
				MsgBox(msgTitle, info);
			}
    #endif
		}
	}
	if (fragData != NULL) 
	{
		oglGetObjectParameteriv( fragId,   GL_OBJECT_COMPILE_STATUS_ARB, &result ); 
		oglGetInfoLog( fragId,   1024, NULL, (char *)info );
		if( !result )
		{
			msys_debugPrintf("FRAG SHADER FAILED: %s\n", info);
			if (strlen(info) > 0)
			{
				sprintf(msgTitle, "FRAG SHADER - %s", name);
				MsgBox(msgTitle, info);
			}
			DebugBreak();
		}else{
    #ifdef SHADER_MSG_ALWAYS
			if (strlen(info) > 0)
			{
				sprintf(msgTitle, "FRAG SHADER - %s", name);
				MsgBox(msgTitle, info);
			}
    #endif
		}
	}
	oglGetObjectParameteriv( progId, GL_OBJECT_LINK_STATUS_ARB, &result ); oglGetInfoLog( progId, 1024, NULL, (char*)info ); 
	if( !result ) 
	{
		msys_debugPrintf("LINK SHADER FAILED: %s\n", info);
		if (strlen(info) > 0)
		{
			sprintf(msgTitle, "LINK SHADER - %s", name);
			MsgBox(msgTitle, info);
		}
		//DebugBreak();
	}else{
    #ifdef SHADER_MSG_ALWAYS
		if (strlen(info) > 0)
		{
			sprintf(msgTitle, "LINK SHADER - %s", name);
			MsgBox(msgTitle, info);
		}
    #endif
    }
    /*
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
	oglGetObjectParameteriv( progId, GL_OBJECT_LINK_STATUS_ARB,    &result ); oglGetInfoLog( progId, 1024, NULL, (char*)info ); 
	if( !result ) 
	{
		//msys_debugPrintf("LINK SHADER FAILED: %s\n", info);
		MessageBox( 0, info, 0, MB_OK|MB_ICONERROR);
		//DebugBreak();
	}	
    */
#endif

}



// Load shader from disk into a null-terminated string
GLchar* ShaderProg::LoadShaderText(const char *fileName, bool localPath)
{	
	#ifdef DEBUG
		char shaderPath[MAX_PATH];
		char path[MAX_PATH];	
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
	#endif
	return 0;	
}



ShaderProg::~ShaderProg(void)
{
}
