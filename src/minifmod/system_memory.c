#include <windows.h>
#include "../sys/msys_types.h"

void fmmemset( void *dst, int val, int amount )
{
    _asm mov edi, dst
    _asm mov eax, val
    _asm mov ecx, amount
    _asm rep stosb
}

extern void msys_memcpy( void *dst, const void *ori, int amount );

void *msys_mallocAlloc( uint32 amount )
{
	//void* ret = HeapAlloc(GetProcessHeap, HEAP_ZERO_MEMORY, amount);
	//return ret;
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
	return GlobalReAlloc(mem, amount, GMEM_MOVEABLE);
}
