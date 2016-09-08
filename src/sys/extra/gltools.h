#include "../msys.h"
#include "../extra/msys_extra.h"
#include "3dHelpers.h"

GLbyte *gltLoadPNG(int resourceId, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

void ViewOrtho(GLdouble width, GLdouble height);
void ViewPerspective();

float ColorByteToFloat(int byteVal);

void _glColor4f(color4f col);
void _glVertex3f(vertex *v);
void _glVertex4f(vertex4 *v);
void _glNormal3f(vertex *v);
void _glTexCoord2f(vertexTexCoord *v);

void populateVertex(vertex* v, float x, float y, float z);
void populateVertex4(vertex4* v, float x, float y, float z, float w);
void populateVertex4(vertex4* v, vertex* v3, float w);
void populateColor3f(color3f* c, float r, float g, float b);
void populateColor4f(color4f* c, float r, float g, float b, float a);
void copyVertex(vertex* vDest, vertex* vSource);
void LogGlError(const char* msg);
void CrossProduct(vertex* out, vertex* v1, vertex* v2);

void loadTexturePng(int resourceId, GLuint* textureId);

// errors
void CheckGlError(bool throwErr);