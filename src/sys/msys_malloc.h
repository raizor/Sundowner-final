//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#ifndef _MSYS_MALLOC_H_
#define _MSYS_MALLOC_H_

#ifndef NULL
#define NULL    0L
#endif

#include "msys_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *msys_mallocAlloc( uint32 amount );
void  msys_mallocFree( void *ptr );
void *msys_mallocRealloc( void* mem, uint32 amount );

#ifdef __cplusplus
}
#endif

int   msys_mallocInit( void );
void  msys_mallocEnd( void );
void *msys_mallocSAlloc( uint32 amount );

inline void *msys_mallocCalloc( uint32 number, uint32 size ) { return(msys_mallocAlloc((uint32)number * (uint32)size)); }


#ifdef WINDOWS
/// just in case somebody wants to program in C++
inline void * __cdecl  operator new( size_t size ) { return(msys_mallocAlloc((uint32)size)); }
inline void   __cdecl  operator delete( void *ptr ) { msys_mallocFree(ptr); }
//void * operator new[] (size_t); 
//void   operator delete[] (void *);


//void * __cdecl operator new( unsigned int size );
//void __cdecl operator delete( void *ptr );
#endif



#endif

