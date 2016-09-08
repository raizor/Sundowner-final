//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#ifndef _INTRO_H_
#define _INTRO_H_

#include "sys/msys.h"

typedef struct
{
    void *obj;
    void (*func)( void *data, int n );  // n=[0..200]
}IntroProgressDelegate;

int  intro_init(int nomusic, IntroProgressDelegate *pd, SCREEN_INFO *screenInfo );
int  intro_do( void );
void intro_end( void );
void intro_reloadShaders();
#endif
