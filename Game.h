#pragma once
#ifndef GAME_H_
#define GAME_H_

#include "Utility.h"
#include "Camera.h"
#include "Terrain.h"




//////////////////////////////////////////////////////////////////////////
//Main game and input functions
//////////////////////////////////////////////////////////////////////////
bool Game_Init();
void Game_Main(float fTimeDelta);
void Game_Shutdown();
bool Init_D3D(D3DDEVTYPE deviceType);
void D3D_Update(float fTimeDelta);
void D3D_Render(float fTimeDelta);

bool Setup();


//////////////////////////////////////////////////////////////////////////
//Callback functions
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////
//Global variables
//////////////////////////////////////////////////////////////////////////
extern IDirect3DDevice9			*g_Device;

extern HINSTANCE				 g_hInstance;

extern HWND						 g_hwnd;

extern bool						g_LMBDown;				//Mouse's left

extern int						g_MouseX, g_MouseY;		

extern	D3DXMATRIX				g_matWorld;

extern	Terrain				   *TheTerrain;

extern	Camera					TheCamera;

#endif