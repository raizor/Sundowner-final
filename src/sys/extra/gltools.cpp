#include <stdlib.h>
#include "gltools.h"
#include "../loaders/stb_image.h"

// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
	GLbyte	identsize;              // Size of ID field that follows header (0)
	GLbyte	colorMapType;           // 0 = None, 1 = paletted
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)


////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *gltLoadPNG(int resourceId, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	HRSRC		rec;
	HGLOBAL		handle;

	LPCSTR lpResourceId = MAKEINTRESOURCE(resourceId);

	rec = FindResource(NULL, lpResourceId, "PNG");
	handle = LoadResource(NULL, rec);

	MEMFILE *memfile;
	memfile = (MEMFILE *)calloc(1, sizeof(MEMFILE));

	memfile->data = LockResource(handle);
	memfile->length = SizeofResource(NULL, rec);
	memfile->pos = 0;

	//stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)';
	int req_comp = 4;
	GLbyte* pixels = (GLbyte*)stbi_load_from_memory((unsigned char*)memfile->data, memfile->length, iWidth, iHeight, iComponents, req_comp);

	*eFormat = GL_RGBA;
	*iComponents = GL_RGBA8;

	return pixels;
}

void ViewOrtho(GLdouble width, GLdouble height)                            // Set Up An Ortho View
{
	glMatrixMode(GL_PROJECTION);                    // Select Projection
	glPushMatrix();                         // Push The Matrix
	glLoadIdentity();                       // Reset The Matrix
	glOrtho( 0, width , height , 0, -1, 1 );             // Select Ortho Mode (ie.640x480)
	glMatrixMode(GL_MODELVIEW);                 // Select Modelview Matrix
	glPushMatrix();                         // Push The Matrix
	glLoadIdentity();                       // Reset The Matrix
}

void ViewPerspective()                          // Set Up A Perspective View
{
	glMatrixMode( GL_PROJECTION );                  // Select Projection
	glPopMatrix();                          // Pop The Matrix
	glMatrixMode( GL_MODELVIEW );                   // Select Modelview
	glPopMatrix();                          // Pop The Matrix

	// todo: reset glViewport
}


float ColorByteToFloat(int byteVal)
{
	return (float)byteVal / 255.0f;
}

void _glColor4f(color4f col)
{
	glColor4f(col.r, col.g, col.b, col.a);
}

void _glVertex3f(vertex *v)
{
	glVertex3f(v->x, v->y, v->z);
}

void _glVertex4f(vertex4 *v)
{
	glVertex4f(v->x, v->y, v->z, v->w);
}

void _glTexCoord2f(vertexTexCoord *v)
{
	glTexCoord2f(v->u, v->v);
}

void _glNormal3f(vertex *v)
{
	glNormal3f(v->x, v->y, v->z);
}

void normalize(vertex *vect)	//scales a vector a length of 1
{
	float length;

	length=msys_sqrtf(					//A^2 + B^2 + C^2 = length^2
		(vect->x*vect->x)+
		(vect->y*vect->y)+
		(vect->z*vect->z)
		);

	vect->x/=length;
	vect->y/=length;
	vect->z/=length;
}

void crossProduct(vertex *c, float a[3], float b[3])  //finds the cross product of two vectors
{  
	c->x=a[1]*b[2] - b[1]*a[2];
	c->y=a[2]*b[0] - b[2]*a[0];
	c->z=a[0]*b[1] - b[0]*a[1];
}

void populateVertex(vertex* v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

void populateVertex4(vertex4* v, vertex* v3, float w)
{
	v->x = v3->x;
	v->y = v3->y;
	v->z = v3->z;
	v->w = w;
}

void populateVertex4(vertex4* v, float x, float y, float z, float w)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
}

void populateColor4f(color4f* c, float r, float g, float b, float a)
{
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = a;
}

void populateColor3f(color3f* c, float r, float g, float b)
{
	c->r = r;
	c->g = g;
	c->b = b;
}

void LogGlError(const char* msg)
{
#ifdef DEBUG
	GLenum e = glGetError();
	if (e>0)
	{
		msys_debugPrintf("GlError: %d = %s\n", e, msg);
	}
#endif
}

void CrossProduct(vertex* out, vertex* a, vertex* b)
{
    out->x = (a->y * b->z) - (a->z * b->y);
    out->y = (a->z * b->x) - (a->x * b->z);
    out->z = (a->x * b->y) - (a->y * b->x);
}

void CheckGlError(bool throwErr)
{
#if DEBUG
	int err = glGetError();
	if (err != 0 && throwErr)
	{
		DebugBreak();
	}
#endif
}


