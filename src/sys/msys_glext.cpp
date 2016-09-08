//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#endif

#include <GL/gl.h>
#include "glext.h"

#ifdef WINDOWS
#endif

#ifdef LINUX
#include <GL/glx.h>
#endif

#ifdef IRIX
#include <GL/glx.h>
#endif

#include <string.h>

//--- d a t a ---------------------------------------------------------------
#include "msys_glext.h"

static char *funciones = {
    // multitexture
    "glActiveTextureARB\x0" // 0
    "glClientActiveTextureARB\x0" // 1
    "glMultiTexCoord4fvARB\x0" // 2

    // programs
    "glDeleteProgramsARB\x0" // 3
    "glBindProgramARB\x0" // 4
    "glProgramStringARB\x0" // 5
    "glProgramLocalParameter4fvARB\x0" // 6
	"glProgramEnvParameter4fvARB\x0" // 7

    // textures 3d
    "glTexImage3D\x0" // 8

    // vbo-ibo
    "glBindBufferARB\x0" // 9
    "glBufferDataARB\x0" // 10
    "glBufferSubDataARB\x0" // 11
    "glDeleteBuffersARB\x0" // 12

	// shader
	"glCreateProgram\x0" // 13
	"glCreateShader\x0" // 14
	"glShaderSource\x0" // 15
	"glCompileShader\x0" // 16
	"glAttachShader\x0" // 17
	"glLinkProgram\x0" // 18
	"glUseProgram\x0" // 19
    "glUniform4fv\x0" // 20
    "glUniform1i\x0" // 21
    "glGetUniformLocationARB\x0" // 22
	"glGetObjectParameterivARB\x0" // 23
	"glGetInfoLogARB\x0" // 24

    "glLoadTransposeMatrixf\x0" // 25

    "glBindRenderbufferEXT\x0" // 26
    "glDeleteRenderbuffersEXT\x0" // 27
    "glRenderbufferStorageEXT\x0" // 28
    "glBindFramebufferEXT\x0" // 29
    "glDeleteFramebuffersEXT\x0" // 30
    "glCheckFramebufferStatusEXT\x0" // 31
    "glFramebufferTexture1DEXT\x0" // 32
    "glFramebufferTexture2DEXT\x0" // 33
    "glFramebufferTexture3DEXT\x0" // 34
    "glFramebufferRenderbufferEXT\x0" // 35
    "glGenerateMipmapEXT\x0" // 36

	// me
	"glMultiTexCoord2fARB\x0" // 37
	"glUniform2fv\x0" // 38
	"glUniform1f\x0" // 39
	"glGenBuffersARB\x0" // 40
	"glGenFramebuffersEXT\x0" // 41
	"glGenRenderbuffersEXT\x0" // 42
	"glDrawBuffers\x0" // 43
	"glUniform2f\x0" // 44
	"glUniform4f\x0" // 45
	"glUniform3f\x0" // 46
	"glUniform2i\x0" // 47
	"glGetUniformBlockIndex\x0" // 48
	"glGetActiveUniformBlockiv\x0" // 49
	"glGetAttribLocation\x0" // 50
	"glVertexAttribPointer\x0" // 51
    };

void *msys_oglfunc[NUMFUNCIONES];

//--- c o d e ---------------------------------------------------------------

int msys_glextInit( void )
{
    char *str = funciones;
    for( int i=0; i<NUMFUNCIONES; i++ )
        {
        #ifdef WINDOWS
        msys_oglfunc[i] = wglGetProcAddress( str );
        #endif
        #ifdef LINUX
        msys_oglfunc[i] = glXGetProcAddress( (const unsigned char *)str );
        #endif
        #ifdef IRIX
        msys_oglfunc[i] = glXGetProcAddress( (const unsigned char *)str );
        #endif

        str += 1+strlen( str );

        if( !msys_oglfunc[i] )
			return( 0 );
        }


    return( 1 );
}


