//--------------------------------------------------------------------------//
// iq . 2003/2008 . code for 64 kb intros by RGBA                           //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>
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
    //---------------
    HINSTANCE    hInstance;
    HDC          hDC;
    HGLRC        hRC;
    HWND         hWnd;
	SCREEN_INFO *screenInfo;
    //---------------
    MSYS_EVENTINFO   events;
}WININFO;

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


static const char fnt_wait[]    = "arial";
static const char msg_wait[]   = "wait while loading...";
static const char msg_error[] = "intro_init()!\n\n"\
                             "  no memory?\n"\
                             "  no music?\n"\
                             "  no sjades?";
static const char wndclass[] = "...";

int loTime = 10000;
int fpsTimes[60];
int fpsIndex=0;
int fpsAvg = 0;

//----------------------------------------------------------------------------

// n = [0..200]
static void loadbar( void *data, int n )
{
    WININFO *info = (WININFO*)data;

    const int xo = (( 28*info->screenInfo->width)>>8);
    const int y1 = ((200*info->screenInfo->height)>>8);
    const int yo = y1-8;

	// draw background
	SelectObject( info->hDC, CreateSolidBrush(0x00000000) );
	Rectangle( info->hDC, 0, 0, info->screenInfo->width, info->screenInfo->height );

	/*
    // draw background
    SelectObject( info->hDC, CreateSolidBrush(0x0045302c) );
    Rectangle( info->hDC, 0, 0, info->screenInfo->width, info->screenInfo->height );

    // draw text
    SetBkMode( info->hDC, TRANSPARENT );
    SetTextColor( info->hDC, 0x00ffffff );
    SelectObject( info->hDC, CreateFont( 44,0,0,0,0,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"impact") );
    TextOut( info->hDC, (info->screenInfo->width-318)>>1, (info->screenInfo->height-38)>>1, "discombobulating...", 21 );

    // draw bar
    SelectObject( info->hDC, CreateSolidBrush(0x00705030) );
    Rectangle( info->hDC, xo, yo, (228*info->screenInfo->width)>>8, y1 );
    SelectObject( info->hDC, CreateSolidBrush(0x00f0d0b0) );
    Rectangle( info->hDC, xo, yo, ((28+n)*info->screenInfo->width)>>8, y1 );*/
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static WININFO     wininfo;

MSYS_EVENTINFO *getEvents( void )
{
    return &wininfo.events;
}

int getKeyPress( int key )
{
    int res;

    res = wininfo.events.keyb.press[key];
    wininfo.events.keyb.press[key] = 0;
    return res;
}


static void procMouse( void )
{
    POINT   p;
    int     i;

    wininfo.events.keyb.state[KEY_LEFT]     = GetAsyncKeyState( VK_LEFT );
	wininfo.events.keyb.state[KEY_RIGHT]    = GetAsyncKeyState( VK_RIGHT );
	wininfo.events.keyb.state[KEY_UP]       = GetAsyncKeyState( VK_UP );
    wininfo.events.keyb.state[KEY_PGUP]     = GetAsyncKeyState( VK_PRIOR );
    wininfo.events.keyb.state[KEY_PGDOWN]   = GetAsyncKeyState( VK_NEXT );
	wininfo.events.keyb.state[KEY_DOWN]     = GetAsyncKeyState( VK_DOWN );
	wininfo.events.keyb.state[KEY_SPACE]    = GetAsyncKeyState( VK_SPACE );
	wininfo.events.keyb.state[KEY_RSHIFT]   = GetAsyncKeyState( VK_RSHIFT );
	wininfo.events.keyb.state[KEY_RCONTROL] = GetAsyncKeyState( VK_RCONTROL );
	wininfo.events.keyb.state[KEY_LSHIFT]   = GetAsyncKeyState( VK_LSHIFT );
	wininfo.events.keyb.state[KEY_LCONTROL] = GetAsyncKeyState( VK_LCONTROL );
	wininfo.events.keyb.state[KEY_1]        = GetAsyncKeyState( '1' );
	wininfo.events.keyb.state[KEY_2]        = GetAsyncKeyState( '2' );
	wininfo.events.keyb.state[KEY_3]        = GetAsyncKeyState( '3' );
	wininfo.events.keyb.state[KEY_4]        = GetAsyncKeyState( '4' );
	wininfo.events.keyb.state[KEY_5]        = GetAsyncKeyState( '5' );
	wininfo.events.keyb.state[KEY_6]        = GetAsyncKeyState( '6' );
	wininfo.events.keyb.state[KEY_7]        = GetAsyncKeyState( '7' );
	wininfo.events.keyb.state[KEY_8]        = GetAsyncKeyState( '8' );
	wininfo.events.keyb.state[KEY_9]        = GetAsyncKeyState( '9' );
	wininfo.events.keyb.state[KEY_0]        = GetAsyncKeyState( '0' );
    for( i=KEY_A; i<=KEY_Z; i++ )
	    wininfo.events.keyb.state[i] = GetAsyncKeyState( 'A'+i-KEY_A );

    //-------
    GetCursorPos( &p );

	wininfo.events.mouse.ox = wininfo.events.mouse.x;
	wininfo.events.mouse.oy = wininfo.events.mouse.y;
	wininfo.events.mouse.x = p.x;
	wininfo.events.mouse.y = p.y;
	wininfo.events.mouse.dx =  wininfo.events.mouse.x - wininfo.events.mouse.ox;
	wininfo.events.mouse.dy =  wininfo.events.mouse.y - wininfo.events.mouse.oy;

	wininfo.events.mouse.obuttons[0] = wininfo.events.mouse.buttons[0];
	wininfo.events.mouse.obuttons[1] = wininfo.events.mouse.buttons[1];
	wininfo.events.mouse.buttons[0] = GetAsyncKeyState(VK_LBUTTON);
	wininfo.events.mouse.buttons[1] = GetAsyncKeyState(VK_RBUTTON);

	wininfo.events.mouse.dbuttons[0] = wininfo.events.mouse.buttons[0] - wininfo.events.mouse.obuttons[0];
	wininfo.events.mouse.dbuttons[1] = wininfo.events.mouse.buttons[1] - wininfo.events.mouse.obuttons[1];
}

//----------------------------------------------------------------------------

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    int i;

	// salvapantallas
	if( uMsg==WM_SYSCOMMAND && (wParam==SC_SCREENSAVE || wParam==SC_MONITORPOWER) )
		return( 0 );

	// boton x o pulsacion de escape
	if( uMsg==WM_CLOSE || uMsg==WM_DESTROY || (uMsg==WM_KEYDOWN && wParam==VK_ESCAPE) )
	{
		PostQuitMessage(0);
        return( 0 );
	}

	if( timeVal >= 167.0)
	{
		PostQuitMessage(0);
		return( 0 );
	}

    if( uMsg==WM_CHAR )
    {
        int conv = 0;
        switch( wParam )
        {
            case VK_LEFT:     conv = KEY_LEFT;     break;
            case VK_RIGHT:    conv = KEY_RIGHT;    break;
            case VK_UP:       conv = KEY_UP;       break;
            case VK_PRIOR:    conv = KEY_PGUP;     break;
            case VK_NEXT:     conv = KEY_PGDOWN;   break;
            case VK_DOWN:     conv = KEY_DOWN;     break;
            case VK_SPACE:    conv = KEY_SPACE;    break;
            case VK_RSHIFT:   conv = KEY_RSHIFT;   break;
            case VK_RCONTROL: conv = KEY_RCONTROL; break;
            case VK_LSHIFT:   conv = KEY_LSHIFT;   break;
            case VK_LCONTROL: conv = KEY_LCONTROL; break;
        }
        
        for( i=KEY_A; i<=KEY_Z; i++ )
        {
            if( wParam==(WPARAM)('A'+i-KEY_A) )
                conv = i;
            if( wParam==(WPARAM)('a'+i-KEY_A) )
                conv = i;
        }

        wininfo.events.keyb.press[conv] = 1;

        if( wParam==VK_ESCAPE)
        {
            PostQuitMessage(0);
            return( 0 );
        }

		if ( wParam == VK_SPACE)
		{
			intro_reloadShaders();
		}

        if( wParam == '1')
        {
            startTime++;
        }

        if( wParam == '2')
        {
            startTime--;
        }

		if( wParam == 't')
		{
			startTime = dTime;
		}

		if( wParam == 'd')
		{
			DebugBreak();
		}
    }

    return( DefWindowProc(hWnd,uMsg,wParam,lParam) );
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

	if( info->screenInfo->fullscreen )
    {
        ChangeDisplaySettings( 0, 0 );
		while( ShowCursor( 1 )<0 ); // show cursor
    }
}

static int window_init( WININFO *info )
{
	unsigned int	PixelFormat;
    DWORD			dwExStyle, dwStyle;
    DEVMODE			dmScreenSettings;
    WNDCLASS		wc;
    RECT			rec;
	
    msys_memset( &wc, 0, sizeof(WNDCLASS) );
    wc.style         = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = info->hInstance;
    wc.lpszClassName = wndclass;
    //wc.hbrBackground=(HBRUSH)CreateSolidBrush(0x00785838);
	
    if( !RegisterClass(&wc) )
        return( 0 );

	if( info->screenInfo->fullscreen )
    {
        msys_memset( &dmScreenSettings,0,sizeof(DEVMODE) );
        dmScreenSettings.dmSize       = sizeof(DEVMODE);
        dmScreenSettings.dmFields     = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		dmScreenSettings.dmBitsPerPel = info->screenInfo->bpp;
        dmScreenSettings.dmPelsWidth  = info->screenInfo->width;
        dmScreenSettings.dmPelsHeight = info->screenInfo->height;

        if( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
            return( 0 );

        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		while( ShowCursor( 0 )>=0 );	// hide cursor
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
    }

    rec.left   = 0;
    rec.top    = 0;
    rec.right  = info->screenInfo->width;
    rec.bottom = info->screenInfo->height;

    AdjustWindowRect( &rec, dwStyle, 0 );

    info->hWnd = CreateWindowEx( dwExStyle, wc.lpszClassName, wc.lpszClassName, dwStyle,
                               (GetSystemMetrics(SM_CXSCREEN)-rec.right+rec.left)>>1,
                               (GetSystemMetrics(SM_CYSCREEN)-rec.bottom+rec.top)>>1,
                               rec.right-rec.left, rec.bottom-rec.top, 0, 0, info->hInstance, 0 );

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
    
    //SetForegroundWindow( info->hWnd );    // slightly higher priority
    //SetFocus( info->hWnd );               // sets keyboard focus to the window
    
    return( 1 );
}


//----------------------------------------------------------------------------

static void DrawTime( WININFO *info, float t )
{
    static int      frame=0;
    static float    to=0.0;
    static int      fps=0;
    char            str[64];
    int             s, m, c;

    if( t<0.0f) return;
	if( info->screenInfo->fullscreen ) return;

    frame++;
    if( (t-to)>1.0f )
    {
        fps = frame;
        to = t;
        frame = 0;
    }

    if( !(frame&3) )
    {
        m = msys_ifloorf( t/60.0f );
        s = msys_ifloorf( t-60.0f*(float)m );
        c = msys_ifloorf( t*100.0f ) % 100;

		long tot = 0;
		for(int i=0; i<60; i++)
		{
			tot+=fpsTimes[i];
		}

		if (fpsIndex == 0)
		{
			fpsAvg = tot/60;
		}
		
		if (fps<loTime && s > 5) loTime = fps;

		fpsTimes[fpsIndex] = fps;
		fpsIndex = fpsIndex < 59 ? fpsIndex + 1 : 0;

        sprintf( str, "%02d:%02d:%02d  [%d fps] [%d avg] [%d low]", m, s, c, fps, (int)fpsAvg, loTime);
        SetWindowText( info->hWnd, str );
    }
}

void calcula( void );

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

			if (!SynthNoteHandled[0] && SynthNotes[0] == 48)
			{
				//DebugBreak();
			
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

static HANDLE threadHandle;

void startMusic()
{

#ifdef ENABLE_MUSIC
#ifdef USE_SOUND_THREAD
	if (threadHandle != 0) TerminateThread(threadHandle, 0);
	// thx to xTr1m/blu-flame for providing a smarter and smaller way to create the thread :)
	threadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_4klang_render, lpSoundBuffer, 0, 0);	
#else
	_4klang_render(lpSoundBuffer);
#endif
#endif
	waveOutOpen			( &hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL );
	waveOutPrepareHeader( hWaveOut, &WaveHDR, sizeof(WaveHDR) );
	waveOutWrite		( hWaveOut, &WaveHDR, sizeof(WaveHDR) );

	musicStarted = true;
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
    MSG         msg;
    int         done=0;

//    calcula(); return( 0 );
    if( !msys_debugInit() )
        return 0;

    wininfo.hInstance = GetModuleHandle( 0 );

    //if( MessageBox( 0, "fullscreen?", info->wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES )
    //   info->full++;

	//globalScreenInfo.fullscreen =1;
	globalScreenInfo.fullscreen = 1;//( MessageBox( 0, "Run Fullscreen?", wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES );
	
	if (globalScreenInfo.fullscreen)
	{
		globalScreenInfo.width = GetSystemMetrics(SM_CXSCREEN);
		globalScreenInfo.height = GetSystemMetrics(SM_CYSCREEN);
        globalScreenInfo.width = 1280;
        globalScreenInfo.height = 720;	
	}else{
		globalScreenInfo.width = 1280;
		globalScreenInfo.height = 720;	
		//globalScreenInfo.width = 1280/2;
		//globalScreenInfo.height = 720/2;		
		//globalScreenInfo.width = (int)(GetSystemMetrics(SM_CXSCREEN));
		//globalScreenInfo.height = (int)(GetSystemMetrics(SM_CYSCREEN));		
	}
    	
	/*
	globalScreenInfo.shaderLevel = ShaderlLevel_All;
	globalScreenInfo.detailLevel = DetailLevel_Medium;*/
	

	wininfo.screenInfo = &globalScreenInfo;
	SelectResolution(instance);

    globalScreenInfo.aspect = (float)globalScreenInfo.width / (float)globalScreenInfo.height;

    if (!globalScreenInfo.quitApp)
    {
        if( !window_init(&wininfo) )
        {
            window_end( &wininfo );
            MessageBox( 0, msg_error,0,MB_OK|MB_ICONEXCLAMATION );
            return( 0 );
        }

        if( !msys_init((intptr)wininfo.hWnd) ) 
        {
            window_end( &wininfo );
            MessageBox( 0, msg_error,0,MB_OK|MB_ICONEXCLAMATION );
            return( 0 );
        }
	
	    for(int i=0; i<17; i++)
	    {
		    SynthNotes[i] = 0;
		    SynthNotesPrev[i] = 0;
		    SynthNoteHandled[i] = false;
	    }

        IntroProgressDelegate pd = { &wininfo, loadbar };
        if( !intro_init( 0, &pd, wininfo.screenInfo ) )
        {
            window_end( &wininfo );
            MessageBox( 0, msg_error, 0, MB_OK|MB_ICONEXCLAMATION );
            return( 0 );
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
                if( msg.message==WM_QUIT) 
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
                procMouse();
                done = intro_do();

			    QueryPerformanceCounter( &currentTicks);
			    QueryPerformanceFrequency( &ticksPerSecond);	

			    dTime = double(currentTicks.QuadPart)/double(ticksPerSecond.QuadPart);
									
			    if( !startTime ) 
			    {
				    startTime=dTime-0;//18;
			    }
                timeVal = (dTime - startTime);			
                SwapBuffers( wininfo.hDC );
			    //glFinish();
                DrawTime( &wininfo, (float)timeVal );


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

        msys_debugEnd();
        timeEndPeriod(1);
    }


    return( 0 );
}



