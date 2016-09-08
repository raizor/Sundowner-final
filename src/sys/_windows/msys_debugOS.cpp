//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#ifdef DEBUG

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "../msys_debug.h"

static FILE *fp;

int msys_debugInit( void )
{
#ifdef DEBUG
    fp = fopen( "debug.txt", "wt" );
    if( !fp )
        return( 0 );

    fprintf( fp, "debug file\n" );
    fprintf( fp, "-------------------------\n" );
    fflush( fp );
    return( 1 );
#else
	return (0);
#endif

}

void msys_debugEnd( void )
{
#ifdef DEBUG
    fprintf( fp, "-------------------------\n" );
    fflush( fp );
    fclose( fp );
#endif
}

void msys_debugPrintf( char *format, ... )
{
#ifdef DEBUG
    va_list arglist;
    va_start( arglist, format );
    vfprintf( fp, format, arglist );
    fflush( fp );
	va_end( arglist );
#endif
}


void msys_debugCheckfor( bool expression, char *format, ... )
{
#ifdef DEBUG
    char    str[1024];

    if( !expression )
        {
        va_list arglist;
        va_start( arglist, format );
        vsprintf( str, format, arglist );
	    va_end( arglist );
        msys_debugHaltProgram( str );
        }
#endif
}

void msys_debugHaltProgram( char *str )
{
    MessageBox( 0, str, "error", MB_OK );
    DebugBreak();
}

#endif
