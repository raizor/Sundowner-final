#ifndef _RES_SELECTOR_H
#define _RES_SELECTOR_H

#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef enum SHADER_LEVEL
{
	ShaderlLevel_All,
	ShaderlLevel_NoPostFx,
	ShaderlLevel_None
}SHADER_LEVEL;

typedef enum DETAIL_LEVEL 
{
	DetailLevel_Low,
	DetailLevel_Medium,
	DetailLevel_High
}DETAIL_LEVEL;

typedef struct
{
    int         width;
	int         height;
	int			bpp;
	int         fullscreen;
	int		    widescreen;
	int			postFx;
	float	    aspect;
	int         fboWidth;
	int         fboHeight;
    int        quitApp;
    int        addNoise;
    int prerenderAudio;
	DETAIL_LEVEL detailLevel;
	SHADER_LEVEL shaderLevel;
}SCREEN_INFO;

int SelectResolution(HINSTANCE hinstance);

extern SCREEN_INFO globalScreenInfo;

#endif