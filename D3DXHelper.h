#pragma once

#include <string> //save error
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <DxErr.h> //get error from error code
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include "Entity.h"
#include "Memory.h"

class D3DXHelper {
private:
	IDirect3D9Ex* d3dObject = NULL; //used to create device
	IDirect3DDevice9Ex* d3dDevice = NULL; //contains functions like begin and end scene 
	D3DPRESENT_PARAMETERS d3dparams; //parameters for creating device
	ID3DXFont* d3dFont = 0; // font used when displaying text
	ID3DXLine* d3dLine = 0;
	HWND targetWnd;
	int width;
	int height;

	D3DXVECTOR2 array[2];
	D3DRECT BarRect;
	
	int radarOffsetX = 200;
	int radarOffsetY = 200;
	int playerSize = 4;
	int directionVecLen = 20;

	int d3D9Init(HWND hWnd);
	void drawText(char* String, int x, int y, D3DCOLOR color);
	void line(int x, int y, int xe, int ye, D3DCOLOR color);
	void rect(int x, int y, int xe, int ye, D3DCOLOR color);
	void rectBordered(int x, int y, int xe, int ye, D3DCOLOR color, D3DCOLOR borderColor);
	
public:
	D3DXHelper();
	D3DXHelper(HWND hWnd, HWND targetWnd, int width, int height);
	int render(Entity* player, Entity* entityList, int entityCnt, Memory mem, float FOV);
	void clear();
	
};