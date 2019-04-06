#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <ctime>
#include <mmsystem.h>
#include <string>
#include <limits>
#include <vector>



#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

// Colors
//
const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));

const D3DXCOLOR BEACH_SAND(D3DCOLOR_XRGB(255, 249, 157));
const D3DXCOLOR DESERT_SAND(D3DCOLOR_XRGB(250, 205, 135));

const D3DXCOLOR LIGHTGREEN(D3DCOLOR_XRGB(60, 184, 120));
const D3DXCOLOR  PUREGREEN(D3DCOLOR_XRGB(0, 166, 81));
const D3DXCOLOR  DARKGREEN(D3DCOLOR_XRGB(0, 114, 54));

const D3DXCOLOR LIGHT_YELLOW_GREEN(D3DCOLOR_XRGB(124, 197, 118));
const D3DXCOLOR  PURE_YELLOW_GREEN(D3DCOLOR_XRGB(57, 181, 74));
const D3DXCOLOR  DARK_YELLOW_GREEN(D3DCOLOR_XRGB(25, 123, 48));

const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
const D3DXCOLOR DARKBROWN(D3DCOLOR_XRGB(115, 100, 87));


#define WINDOW_WIDTH	 800
#define WINDOW_HEIGHT	 600
#define WINDOW_TITLE	 "寻仙"

//Set the Mouse button states
#define LEFT_BUTTON		0
#define RIGHT_BUTTON	1

//Help macros
#define  _CRT_SECIRE_NO_DEPRECATE
#ifndef HR
#define HR(x)    { hr = x; if( FAILED(hr) ) { return hr; } }         //自定义一个HR宏，方便执行错误的返回
#endif

#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }       //自定义一个SAFE_RELEASE()宏,便于指针资源的释放
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }     //自定义一个SAFE_RELEASE()宏,便于COM资源的释放
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    