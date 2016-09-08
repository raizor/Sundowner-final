//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include "../msys_types.h"
#include "../msys_types.h"

/*
extern void msys_memcpy( void *dst, const void *ori, int amount );

void *msys_mallocAlloc( uint32 amount )
{
	/
	HANDLE heap = GetProcessHeap();
	void* ret = HeapAlloc(heap, HEAP_ZERO_MEMORY, amount);
	return ret;
	return( GlobalAlloc( GMEM_ZEROINIT, amount ) );
}

void msys_mallocFree( void *ptr )
{
	//HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, ptr);
	GlobalFree( ptr );
}

void *msys_mallocRealloc( void* mem, uint32 amount )
{
	if (mem == NULL)
	{
		return msys_mallocAlloc(amount);
	}
	return( GlobalReAlloc(mem, amount, GMEM_FIXED) );
}*/

