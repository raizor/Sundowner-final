//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA 
// Malloc stuff for C (rather than CPP)
//--------------------------------------------------------------------------//

#ifndef _MSYS_MALLOC_C_H_
#define _MSYS_MALLOC_C_H_

#ifndef NULL
#define NULL    0L
#endif

#include "msys_types.h"

void *msys_mallocAlloc( uint32 amount );
void  msys_mallocFree( void *ptr );
void *msys_mallocRealloc( void* mem, uint32 amount );


#endif

