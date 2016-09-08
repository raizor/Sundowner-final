#include "stacks.h"

/////////////////////////////

objectStack::objectStack(int size)
{
	max = size;
	items = new void*[max];	
	count = 0;
}

void objectStack::push(void* i)
{
	items[count++] = i;
}

void objectStack::pop()
{
	count--;
}

objectStack::~objectStack(void)
{
	delete(items);
}




