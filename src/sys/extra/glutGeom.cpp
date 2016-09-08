#include "glutGeom.h"

/*
 * Compute lookup table of cos and sin values forming a cirle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */

static void fgError(char* txt)
{
#ifdef DEBUG
	msys_debugPrintf(txt);
#endif
}

void fghCircleTable(double **sint,double **cost,const int n)
{
    int i;

    /* Table size, the sign of n flips the circle direction */

    const int size = (int)msys_fabsf((float)n);

    /* Determine the angle between samples */

    const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

    /* Allocate memory for n samples, plus duplicate of first entry at the end */

    *sint = (double *) msys_mallocCalloc(sizeof(double), size+1);
    *cost = (double *) msys_mallocCalloc(sizeof(double), size+1);

    /* Bail out if memory allocation fails, fgError never returns */

    if (!(*sint) || !(*cost))
    {
        msys_mallocFree(*sint);
        msys_mallocFree(*cost);
        fgError("Failed to allocate memory in fghCircleTable");
    }

    /* Compute cos and sin around the circle */

    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = msys_sinf((float)angle*i);
        (*cost)[i] = msys_cosf((float)angle*i);
    }

    /* Last sample is duplicate of the first */

    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}


/*
 * Draws a wire sphere
 */
void glutWireSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks and slices are drawn. */

    double r;
    double x,y,z;

    /* Pre-computed circle */

    double *sint1,*cost1;
    double *sint2,*cost2;

    fghCircleTable(&sint1,&cost1,-slices  );
    fghCircleTable(&sint2,&cost2, stacks*2);

    /* Draw a line loop for each stack */

    for (i=1; i<stacks; i++)
    {
        z = cost2[i];
        r = sint2[i];

        glBegin(GL_LINE_LOOP);

            for(j=0; j<=slices; j++)
            {
                x = cost1[j];
                y = sint1[j];

                glNormal3d(x,y,z);
                glVertex3d(x*r*radius,y*r*radius,z*radius);
            }

        glEnd();
    }

    /* Draw a line loop for each slice */

    for (i=0; i<slices; i++)
    {
        glBegin(GL_LINE_STRIP);

            for(j=0; j<=stacks; j++)
            {
                x = cost1[i]*sint2[j];
                y = sint1[i]*sint2[j];
                z = cost2[j];

                glNormal3d(x,y,z);
                glVertex3d(x*radius,y*radius,z*radius);
            }

        glEnd();
    }

    /* Release sin and cos tables */

    msys_mallocFree(sint1);
    msys_mallocFree(cost1);
    msys_mallocFree(sint2);
    msys_mallocFree(cost2);
}

/*
 * Draws a solid sphere
 */
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

    /* Adjust z and radius as stacks are drawn. */

    double z0,z1;
    double r0,r1;

    /* Pre-computed circle */

    double *sint1,*cost1;
    double *sint2,*cost2;

    //FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidSphere" );

    fghCircleTable(&sint1,&cost1,-slices);
    fghCircleTable(&sint2,&cost2,stacks*2);

    /* The top stack is covered with a triangle fan */

    z0 = 1.0;
    z1 = cost2[(stacks>0)?1:0];
    r0 = 0.0;
    r1 = sint2[(stacks>0)?1:0];

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,1);
        glVertex3d(0,0,radius);

        for (j=slices; j>=0; j--)
        {
            glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
            glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
        }

    glEnd();

    /* Cover each stack with a quad strip, except the top and bottom stacks */

    for( i=1; i<stacks-1; i++ )
    {
        z0 = z1; z1 = cost2[i+1];
        r0 = r1; r1 = sint2[i+1];

        glBegin(GL_QUAD_STRIP);

            for(j=0; j<=slices; j++)
            {
                glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
                glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
                glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
                glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
            }

        glEnd();
    }

    /* The bottom stack is covered with a triangle fan */

    z0 = z1;
    r0 = r1;

    glBegin(GL_TRIANGLE_FAN);

        glNormal3d(0,0,-1);
        glVertex3d(0,0,-radius);

        for (j=0; j<=slices; j++)
        {
            glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
            glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
        }

    glEnd();

    /* Release sin and cos tables */

    msys_mallocFree(sint1);
    msys_mallocFree(cost1);
    msys_mallocFree(sint2);
    msys_mallocFree(cost2);
}


/*
 * Draws a wire torus
 */
void glutWireTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings )
{
  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
  double *vertex, *normal;
  int    i, j;
  double spsi, cpsi, sphi, cphi ;

  //FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutWireTorus" );

  if ( nSides < 1 ) nSides = 1;
  if ( nRings < 1 ) nRings = 1;

  /* Allocate the vertices array */
  vertex = (double *)msys_mallocCalloc( sizeof(double), 3 * nSides * nRings );
  normal = (double *)msys_mallocCalloc( sizeof(double), 3 * nSides * nRings );

  glPushMatrix();

  dpsi =  2.0 * M_PI / (double)nRings ;
  dphi = -2.0 * M_PI / (double)nSides ;
  psi  = 0.0;

  for( j=0; j<nRings; j++ )
  {
    cpsi = msys_cosf ( (float)psi ) ;
    spsi = msys_sinf ( (float)psi ) ;
    phi = 0.0;

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      cphi = msys_cosf ( (float)phi ) ;
      sphi = msys_sinf ( (float)phi ) ;
      *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 2) =                    sphi * iradius  ;
      *(normal + offset + 0) = cpsi * cphi ;
      *(normal + offset + 1) = spsi * cphi ;
      *(normal + offset + 2) =        sphi ;
      phi += dphi;
    }

    psi += dpsi;
  }

  for( i=0; i<nSides; i++ )
  {
    glBegin( GL_LINE_LOOP );

    for( j=0; j<nRings; j++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
    }

    glEnd();
  }

  for( j=0; j<nRings; j++ )
  {
    glBegin(GL_LINE_LOOP);

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
    }

    glEnd();
  }

  msys_mallocFree ( vertex ) ;
  msys_mallocFree ( normal ) ;
  glPopMatrix();
}

/*
 * Draws a solid torus
 */
void glutSolidTorus( GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings )
{
  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
  double *vertex, *normal;
  int    i, j;
  double spsi, cpsi, sphi, cphi ;

  //FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidTorus" );

  if ( nSides < 1 ) nSides = 1;
  if ( nRings < 1 ) nRings = 1;

  /* Increment the number of sides and rings to allow for one more point than surface */
  nSides ++ ;
  nRings ++ ;

  /* Allocate the vertices array */
  vertex = (double *)msys_mallocCalloc( sizeof(double), 3 * nSides * nRings );
  normal = (double *)msys_mallocCalloc( sizeof(double), 3 * nSides * nRings );

  glPushMatrix();

  dpsi =  2.0 * M_PI / (double)(nRings - 1) ;
  dphi = -2.0 * M_PI / (double)(nSides - 1) ;
  psi  = 0.0;

  for( j=0; j<nRings; j++ )
  {
    cpsi = msys_cosf ( (float)psi ) ;
    spsi = msys_sinf ( (float)psi ) ;
    phi = 0.0;

    for( i=0; i<nSides; i++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      cphi = msys_cosf ( (float)phi ) ;
      sphi = msys_sinf ( (float)phi ) ;
      *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 2) =                    sphi * iradius  ;
      *(normal + offset + 0) = cpsi * cphi ;
      *(normal + offset + 1) = spsi * cphi ;
      *(normal + offset + 2) =        sphi ;
      phi += dphi;
    }

    psi += dpsi;
  }

    glBegin( GL_QUADS );
  for( i=0; i<nSides-1; i++ )
  {
    for( j=0; j<nRings-1; j++ )
    {
      int offset = 3 * ( j * nSides + i ) ;
      glNormal3dv( normal + offset );
      glVertex3dv( vertex + offset );
      glNormal3dv( normal + offset + 3 );
      glVertex3dv( vertex + offset + 3 );
      glNormal3dv( normal + offset + 3 * nSides + 3 );
      glVertex3dv( vertex + offset + 3 * nSides + 3 );
      glNormal3dv( normal + offset + 3 * nSides );
      glVertex3dv( vertex + offset + 3 * nSides );
    }
  }

  glEnd();

  msys_mallocFree ( vertex ) ;
  msys_mallocFree ( normal ) ;
  glPopMatrix();
}