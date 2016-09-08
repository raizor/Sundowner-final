#pragma once

#include <stdio.h>

typedef struct 
{
	int length;
	int pos;
	void *data;
} MEMFILE;

void memclose(unsigned int handle);
void msys_memcpy( void *dst, const void *ori, int amount );
void memseek(unsigned int handle, int pos, signed char mode);
int memtell(unsigned int handle);