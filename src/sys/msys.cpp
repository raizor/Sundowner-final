//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#include "msys_glext.h"
#include "msys_malloc.h"
#include "msys_font.h"
#include "msys_globals.h"

int msys_init( uint64 h )
{
    if( !msys_glextInit() ) return 0;
    if( !msys_mallocInit() ) return 0;

	globalFboManager = new fboManager();

    return 1;
}

void msys_end( void )
{
    msys_mallocEnd();
}