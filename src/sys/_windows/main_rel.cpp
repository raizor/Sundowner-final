// /CRINKLER /PRINT:LABELS /PRINT:IMPORTS /COMPMODE:SLOW /HASHSIZE:200 /ORDERTRIES:6000  /UNSAFEIMPORT

//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#ifdef A64BITS
#pragma pack(8) // VERY important, so WNDCLASS get's the correct padding and we don't crash the system
#endif

//#pragma check_stack(off)
//#pragma runtime_checks("", off)


#define ALLOWWINDOWED       // allow windowed mode

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <string.h>
#include "../../intro.h"
#include "../msys.h"
#include "../events.h"
#include "../resSelector.h"
#include "../../4klang.h"
#include "../../config.h"

int SynthNotesPrev[17];
int SynthNotes[17];
int SynthNoteCounters[17];
bool SynthNoteHandled[17];
bool quitting = false;
bool quit = false;
float quitValX = 1.0f;
float quitValY = 1.0f;
double timeVal = 0.0f;
double quitTime = 0.0;
double dTime = 0.0;
LARGE_INTEGER ticksPerSecond;
LARGE_INTEGER currentTicks;
bool musicStarted = false;
static double startTime = 0; 

//==============================================================================================

// MAX_SAMPLES gives you the number of samples for the whole song. we always produce stereo samples, so times 2 for the buffer
SAMPLE_TYPE	lpSoundBuffer[MAX_SAMPLES*2];  
HWAVEOUT	hWaveOut;

#pragma data_seg(".wavefmt")
WAVEFORMATEX WaveFMT =
{
#ifdef FLOAT_32BIT	
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif		
	2, // channels
	SAMPLE_RATE, // samples per sec
	SAMPLE_RATE*sizeof(SAMPLE_TYPE)*2, // bytes per sec
	sizeof(SAMPLE_TYPE)*2, // block alignment;
	sizeof(SAMPLE_TYPE)*8, // bits per sample
	0 // extension not needed
};

#pragma data_seg(".wavehdr")
WAVEHDR WaveHDR = 
{
	(LPSTR)lpSoundBuffer, 
	MAX_SAMPLES*sizeof(SAMPLE_TYPE)*2,			// MAX_SAMPLES*sizeof(float)*2(stereo)
	0, 
	0, 
	0, 
	0, 
	0, 
	0
};

MMTIME MMTime = 
{ 
	TIME_SAMPLES,
	0
};


//----------------------------------------------------------------------------

typedef struct
{
    HINSTANCE   hInstance;
    HWND        hWnd;
    HDC         hDC;
    HGLRC       hRC;
	SCREEN_INFO *screenInfo;
}WININFO;

extern "C" int _fltused = 0;

static const PIXELFORMATDESCRIPTOR pfd =
{
    sizeof(PIXELFORMATDESCRIPTOR),
    1,
    PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    32,             // zbuffer
    0,              // stencil!
    0,
    PFD_MAIN_PLANE,
    0, 0, 0, 0
};

static DEVMODE screenSettings = { {0},
    #if _MSC_VER < 1400
    0,0,148,0,0x001c0000,{0},0,0,0,0,0,0,0,0,0,{0},0,32,XRES,YRES,0,0,      // Visual C++ 6.0
    #else
    0,0,156,0,0x001c0000,{0},0,0,0,0,0,{0},0,32,0,0,{0}, 0,           // Visuatl Studio 2005
    #endif
    #if(WINVER >= 0x0400)
    0,0,0,0,0,0,
    #if (WINVER >= 0x0500) || (_WIN32_WINNT >= 0x0400)
    0,0
    #endif
    #endif
    };
static const char wndclass[] = "Sundowner";
static const char msg_error[] = "Something went bang...";

static bool closing = false;


//----------------------------------------------------------------------------
static void loadbar( void *data, int n )
{
	/*
    WININFO *info = (WININFO*)data;

    const int xo = (( 28*info->screenInfo->width)>>8);
    const int y1 = ((200*info->screenInfo->height)>>8);
    const int yo = y1-8;

    // draw background
    SelectObject( info->hDC, CreateSolidBrush(0x0045302c) );
    Rectangle( info->hDC, 0, 0, info->screenInfo->width, info->screenInfo->height );

    // draw text
    SetBkMode( info->hDC, TRANSPARENT );
    SetTextColor( info->hDC, 0x00ffffff );
    SelectObject( info->hDC, CreateFont( 44,0,0,0,0,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"arial") );
    TextOut( info->hDC, (info->screenInfo->width-318)>>1, (info->screenInfo->height-38)>>1, "Gobbling...", 21 );

    // draw bar
    SelectObject( info->hDC, CreateSolidBrush(0x00705030) );
    Rectangle( info->hDC, xo, yo, (228*info->screenInfo->width)>>8, y1 );
    SelectObject( info->hDC, CreateSolidBrush(0x00f0d0b0) );
    Rectangle( info->hDC, xo, yo, ((28+n)*info->screenInfo->width)>>8, y1 );*/
}

//----------------------------------------------------------------------------

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if( timeVal >= 167.0 && !closing)
    {
        closing = true;
        PostQuitMessage(0);
        return( 0 );
    }

	if( uMsg==WM_SYSCOMMAND && (wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER) )
		return 0 ;

    if( (uMsg==WM_CLOSE || (uMsg==WM_KEYDOWN && wParam==VK_ESCAPE)) && !closing )
    {
        closing = true;
        PostQuitMessage(0);
        return 0 ;
    }

    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

static void window_end( WININFO *info )
{
    if( info->hRC )
    {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( info->hRC );
    }

    if( info->hDC  ) ReleaseDC( info->hWnd, info->hDC );
    if( info->hWnd ) DestroyWindow( info->hWnd );

    UnregisterClass( wndclass, info->hInstance );

    #ifdef ALLOWWINDOWED
	if( info->screenInfo->fullscreen )
    #endif
    {
        ChangeDisplaySettings( 0, 0 );
		ShowCursor( 1 ); 
    }
}

static int window_init( WININFO *info )
{
	unsigned int	PixelFormat;
    DWORD			dwExStyle, dwStyle;
    RECT			rec;
    WNDCLASSA		wc;

    info->hInstance = GetModuleHandle( 0 );

    msys_memset( &wc, 0, sizeof(WNDCLASSA) );

    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = info->hInstance;
    wc.lpszClassName = wndclass;

    if( !RegisterClass((WNDCLASSA*)&wc) )
        return( 0 );

    #ifdef ALLOWWINDOWED
	if( info->screenInfo->fullscreen )
    #endif
    {
		screenSettings.dmPelsWidth = info->screenInfo->width;
		screenSettings.dmPelsHeight = info->screenInfo->height;
        if( ChangeDisplaySettings(&screenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return( 0 );
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		ShowCursor( 0 );
    }
    #ifdef ALLOWWINDOWED
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;
        dwStyle   = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    }
    #endif
    rec.left   = 0;
    rec.top    = 0;
    rec.right  = info->screenInfo->width;
    rec.bottom = info->screenInfo->height;

    #ifdef ALLOWWINDOWED
    AdjustWindowRect( &rec, dwStyle, 0 );
    info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, wc.lpszClassName, dwStyle,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );
    #else
    info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, wc.lpszClassName, dwStyle, 0, 0, 
                                 rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );
    #endif

    if( !info->hWnd )
        return( 0 );

    if( !(info->hDC=GetDC(info->hWnd)) )
        return( 0 );

    if( !(PixelFormat=ChoosePixelFormat(info->hDC,&pfd)) )
        return( 0 );

    if( !SetPixelFormat(info->hDC,PixelFormat,&pfd) )
        return( 0 );

    if( !(info->hRC=wglCreateContext(info->hDC)) )
        return( 0 );

    if( !wglMakeCurrent(info->hDC,info->hRC) )
        return( 0 );
    
    SetForegroundWindow(info->hWnd);
    SetFocus(info->hWnd);

    return( 1 );
}


//----------------------------------------------------------------------------
#if 0
extern "C" extern int __cdecl _heap_init (int);
extern "C" extern int __cdecl _mtinit ( void );
extern "C" _CRTIMP int __cdecl _CrtSetCheckCount(int);
extern "C" extern int __cdecl _ioinit (void);
extern "C" extern int __cdecl _cinit (int);



/*
extern "C" extern int _heap_init(int);
extern "C" extern void _ioinit(void);
extern "C" extern void _cinit(void);

extern "C" extern void _mtinit(void);
*/

#include <rtcapi.h>
extern "C" extern void _RTC_Initialize(void);


int __cdecl MyErrorFunc(int, const wchar_t *, int, const wchar_t *, const wchar_t *, ...)
{
MessageBox(0,"q",0,0);
    return 0;
}


/*
// C version:
_RTC_error_fnW __cdecl _CRT_RTC_INITW(void *res0, void **res1, int res2, int res3, int res4)
{
    return &MyErrorFunc; 
}
*/

// C++ version:
extern "C" _RTC_error_fnW __cdecl _CRT_RTC_INITW(void *res0, void **res1, int res2, int res3, int res4)
{
    return &MyErrorFunc;
}

#include <winbase.h>

 // RunTmChk.lib
#endif


void handleSync()
{
	if (!musicStarted) return;
	// get sample position for timing
		waveOutGetPosition(hWaveOut, &MMTime, sizeof(MMTIME));

		
		// access to note buffer for sync. only works when the song was exported with that option
		// notes are recorded for each voice (max 2) in each instrument (max 16) every 256 samples.
		// so, e.g. to get note of instrument 5, voice 0 do:
		//int aha = (&_4klang_note_buffer)[((MMTime.u.sample >> 8) << 5) + 2*5+0];
		// so, e.g. to get note of instrument 3, voice 1 do:
		//int oho = (&_4klang_note_buffer)[((MMTime.u.sample >> 8) << 5) + 2*3+1];

		for(int i=0; i<16; i++)
		{
			// small example showing how to access the current note for a selected instrument (instrument 1 in this case)
			int curNote = (&_4klang_note_buffer)[((MMTime.u.sample >> 8) << 5) + 2*i+0]; 

			if (curNote!=0 && curNote != 60)
			{
				int inst = i;
				int x =1;

			}

			if (curNote != SynthNotes[i])
			{
				SynthNotesPrev[i] = SynthNotes[i];
				SynthNotes[i] = curNote;
				SynthNoteHandled[i] = false;
				SynthNoteCounters[i] = 0;
			}
			/*
			static bool on = 0;
			if (!on && (curNote == 52))
			{
				on = true;
			}
			if (curNote != 52)
			{
				on = false;
			}*/
		}
}

void startMusic()
{

#ifdef ENABLE_MUSIC
if (!globalScreenInfo.prerenderAudio)
{
	// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);
}else{
	_4klang_render(lpSoundBuffer);
}
#endif
	waveOutOpen			( &hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL );
	waveOutPrepareHeader( hWaveOut, &WaveHDR, sizeof(WaveHDR) );
	waveOutWrite		( hWaveOut, &WaveHDR, sizeof(WaveHDR) );

	musicStarted = true;
}

void entrypoint( void )
{


    WININFO     wininfo;
    MSG         msg;
    int         done=0;

	for(int i=0; i<17; i++)
	{
		SynthNotes[i] = 0;
		SynthNotesPrev[i] = 0;
		SynthNoteHandled[i] = false;
	}


    #ifdef ALLOWWINDOWED

		globalScreenInfo.fullscreen = 0;
		globalScreenInfo.fullscreen = 1;//( MessageBox( 0, "Run Fullscreen?", wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES );
		if (globalScreenInfo.fullscreen)
		{
			globalScreenInfo.width = GetSystemMetrics(SM_CXSCREEN);
			globalScreenInfo.height = GetSystemMetrics(SM_CYSCREEN);
		}else{
			globalScreenInfo.width = 1280;
			globalScreenInfo.height = 720;	
			//globalScreenInfo.width = (int)(GetSystemMetrics(SM_CXSCREEN));
			//globalScreenInfo.height = (int)(GetSystemMetrics(SM_CYSCREEN));		
		}

		globalScreenInfo.aspect = (float)globalScreenInfo.width / (float)globalScreenInfo.height;

		globalScreenInfo.bpp = 32;

		globalScreenInfo.fboWidth = 512; 
		globalScreenInfo.fboHeight = 512;
		
		wininfo.screenInfo = &globalScreenInfo;
	
		SelectResolution(0);
    #endif

    if (!globalScreenInfo.quitApp)
    {
        
        if( !window_init(&wininfo) )
        {
            window_end( &wininfo );
            MessageBox( 0,msg_error,0,MB_OK|MB_ICONEXCLAMATION );
            ExitProcess( 0 );
        }

        if( !msys_init((intptr)wininfo.hWnd) ) 
        {
            window_end( &wininfo );
            MessageBox( 0,msg_error,0,MB_OK|MB_ICONEXCLAMATION );
            ExitProcess( 0 );
        }

        IntroProgressDelegate pd = { &wininfo, loadbar };
        if( !intro_init( 0, &pd, wininfo.screenInfo ) )
        {
            window_end( &wininfo );
            MessageBox( 0,msg_error,0,MB_OK|MB_ICONEXCLAMATION );
            ExitProcess( 0 );
        }

	    //if ((timeVal>0 && !musicStarted))
	    //{
	    startMusic();
	    //}

	    timeBeginPeriod(1);
	    DWORD_PTR mask = 1; 
	    SetThreadAffinityMask(GetCurrentThread(), mask);

        while( !done )
        {
		    handleSync();
		
            if( PeekMessage(&msg,0,0,0,PM_REMOVE) )
            {
                if( msg.message==WM_QUIT && !quitting ) 
			    {
				    if (timeVal < 10.0)
				    {
					    // no post fx exit, just quit
					    done = 1;
				    }else{
					    quitting =true;
					    quitTime = timeVal;
				    }
				    //done=1;
			    }
		        TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
            {
                //procMouse();
                done = intro_do();

			    QueryPerformanceCounter( &currentTicks);
			    QueryPerformanceFrequency( &ticksPerSecond);	

			    dTime = double(currentTicks.QuadPart)/double(ticksPerSecond.QuadPart);
									
			    if( !startTime ) 
			    {
				    startTime=dTime-0;
			    }
                timeVal = (dTime - startTime);			
                SwapBuffers( wininfo.hDC );
			    //glFinish();
                //DrawTime( &wininfo, (float)timeVal );


			    handleSync();
			    //msys_timerSleep(10);
            }

            if (quit) done = 1;
		
		    /*
		    static unsigned int msThisFrame = msys_timerGet()-startTime; 
		    if(msThisFrame < 16)
		    {
			    Sleep(16-msThisFrame);
		    }*/
        }

        intro_end();

        window_end( &wininfo );

        msys_end();
    }

    ExitProcess( 0 );
}
