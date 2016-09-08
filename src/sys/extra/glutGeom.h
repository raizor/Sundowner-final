#include "../msys.h"

void fghCircleTable(double **sint,double **cost,const int n);

void glutWireSphere(GLdouble radius, GLint slices, GLint stacks);
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks);
void glutSolidTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings );
void glutWireTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings );