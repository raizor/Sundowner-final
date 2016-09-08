#include "msys.h"
#include "Shlwapi.h"
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "../resources/resource.h"

void memclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	free(memfile);
}

void msys_memcpy( void *dst, const void *ori, int amount )
{
	_asm mov edi, dst
		_asm mov esi, ori
		_asm mov ecx, amount
		_asm rep movsb
}


void memseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (mode == SEEK_SET) 
		memfile->pos = pos;
	else if (mode == SEEK_CUR) 
		memfile->pos += pos;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + pos;

	if (memfile->pos > memfile->length)
		memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	return memfile->pos;
}
