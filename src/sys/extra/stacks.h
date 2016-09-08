#pragma once

#include "../msys.h"
#include "msys_extra.h"

class objectStack
{
public:
	int max;
	int count;	
	void** items;
	void objectStack::pop();
	void objectStack::push(void* index);
	objectStack(int size);
	~objectStack(void);
};
