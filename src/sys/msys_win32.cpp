#include "msys_win32.h"

void MsgBox(const char* title, const char* msg)
{
	MessageBox( 0, msg, title, MB_OK | MB_TOPMOST );
}