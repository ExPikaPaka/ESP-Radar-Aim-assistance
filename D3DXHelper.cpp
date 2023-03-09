#include "D3DXHelper.h"
#include <iostream>
#include <math.h>

int D3DXHelper::d3D9Init(HWND hWnd) {

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject))) {
		exit(1);
	}

	ZeroMemory(&d3dparams, sizeof(d3dparams));

	d3dparams.BackBufferWidth = width;
	d3dparams.BackBufferHeight = height;
	d3dparams.Windowed = TRUE;
	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.hDeviceWindow = hWnd;
	d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dparams.EnableAutoDepthStencil = TRUE;
	d3dparams.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT res = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);

	if (FAILED(res)) {
		std::wstring ws(DXGetErrorString(res));
		std::string str(ws.begin(), ws.end());
		std::wstring ws2(DXGetErrorDescription(res));
		std::string str2(ws2.begin(), ws2.end());
		std::string error = "Error: " + str + " error description: " + str2;
		exit(1);
	}

	D3DXCreateFont(d3dDevice, 25, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Comic Sans", &d3dFont);

	return 0;

}

D3DXHelper::D3DXHelper() {};

D3DXHelper::D3DXHelper(HWND hWnd, HWND targetWnd, int width, int height) {
	this->width = width;
	this->height = height;
	this->targetWnd = targetWnd;
	d3D9Init(hWnd);
}

void rotate_point(Entity* base, Entity* toRotate, float angle) {
	angle *= 3.141592 / 180;
	float s = sin(angle);
	float c = cos(angle);

	// translate point back to origin:
	toRotate->x -= base->x;
	toRotate->y -= base->y;

	// rotate point
	float xnew = toRotate->x * c - toRotate->y * s;
	float ynew = toRotate->x * s + toRotate->y * c;

	// translate point back:
	toRotate->x = xnew + base->x;
	toRotate->y = ynew + base->y;
}

void rotate(float &x, float &y, float &z, float &xC, float &yC, float &zC, float angleXY, float angleXZ, float angleYZ) {
	angleXY *= 3.141592 / 180;
	angleXZ *= 3.141592 / 180;
	angleYZ *= 3.141592 / 180;
	if (angleXY) {
		float s = sin(angleXY);  x -= xC;
		float c = cos(angleXY);  y -= yC;

		float xnew = x * c - y * s;
		float ynew = x * s + y * c;

		x = xnew + xC;
		y = ynew + yC;
	}
	if (angleXZ) {
		float s = sin(angleXZ);  x -= xC;
		float c = cos(angleXZ);  z -= zC;

		float xnew = x * c - z * s;
		float ynew = x * s + z * c;

		x = xnew + xC;
		z = ynew + zC;
	}
	if (angleYZ) {
		float s = sin(angleXY);  y -= xC;
		float c = cos(angleXY);  z -= yC;

		float xnew = y * c - z * s;
		float ynew = y * s + z * c;

		y = xnew + xC;
		z = ynew + yC;
	}
}

float X = 10, Y = 10, Z = 15;

float Xo = 200, Yo = 200, Zo = 200;

float angleToRad(float angle) {
	return angle * (3.141592 / 180.0);
}

int D3DXHelper::render(Entity* player, Entity *entityList, int entityCnt, Memory mem, float FOV) {
	if (d3dDevice == nullptr)
		return 1;
	//d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
	d3dDevice->BeginScene();

	if (targetWnd == GetForegroundWindow())	{
		if (player->team) {
			rectBordered(radarOffsetX - playerSize, radarOffsetY - playerSize,
				radarOffsetX + playerSize, radarOffsetY + playerSize,
				D3DCOLOR_ARGB(255, 100, 100, 255), D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else {
			rectBordered(radarOffsetX - playerSize, radarOffsetY - playerSize,
				radarOffsetX + playerSize, radarOffsetY + playerSize,
				D3DCOLOR_ARGB(255, 255, 100, 100), D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		
		line(player->x + radarOffsetX - player->x,
			player->y + radarOffsetY - player->y,
			player->x + radarOffsetX - player->x + cos((-90) * (3.141592 / 180.0))* directionVecLen,
			player->y + radarOffsetY - player->y + sin((-90) * (3.141592 / 180.0))* directionVecLen,
			D3DCOLOR_ARGB(255, 255, 255, 255));

	
		float angleBetweenUs = 0;
		float angleBetweenVerts = 0;
		float smallestX = 55555;
		float smallestY = 55555;
		
		bool flag = 0;

		for (int j = 0; j < entityCnt; j++) {
			if (entityList[j].isGood) {
				if (entityList[j].hp > 0) {

					//////////// RADARHACK REGION START
					rotate_point(player, (entityList + j), -player->yaw);
					entityList[j].yaw -= player->yaw;

					/*if (entityList[j].team) {
						rectBordered(entityList[j].x - playerSize + radarOffsetX - player->x, entityList[j].y - playerSize + radarOffsetY - player->y,
							entityList[j].x + playerSize + radarOffsetX - player->x, entityList[j].y + playerSize + radarOffsetY - player->y,
							D3DCOLOR_ARGB(255, 0, 0, 255), D3DCOLOR_ARGB(255, 0, 0, 255));
					}
					else {
						rectBordered(entityList[j].x - playerSize + radarOffsetX - player->x, entityList[j].y - playerSize + radarOffsetY - player->y,
							entityList[j].x + playerSize + radarOffsetX - player->x, entityList[j].y + playerSize + radarOffsetY - player->y,
							D3DCOLOR_ARGB(255, 255, 0, 0), D3DCOLOR_ARGB(255, 255, 0, 0));
					}

					line(entityList[j].x + radarOffsetX - player->x,
						entityList[j].y + radarOffsetY - player->y,
						entityList[j].x + radarOffsetX - player->x + cos((entityList[j].yaw - 90) * (3.141592 / 180.0)) * directionVecLen,
						entityList[j].y + radarOffsetY - player->y + sin((entityList[j].yaw - 90) * (3.141592 / 180.0)) * directionVecLen,
						D3DCOLOR_ARGB(255, 255, 255, 255));*/
					
					rotate_point(player, (entityList + j), player->yaw);
					//////////// RADARHACK REGION END


					//////////// ESP REGION START
					rotate_point(player, (entityList + j), -player->yaw);
					angleBetweenUs = atan((player->x - entityList[j].x) / (player->y - entityList[j].y)) / (3.141592 / 180.0);
					if ((entityList[j].y > player->y) && (entityList[j].x < player->x)) angleBetweenUs += 180;
					if ((entityList[j].y > player->y) && (entityList[j].x > player->x)) angleBetweenUs -= 180;

					angleBetweenVerts = -atan((player->z - entityList[j].z) / sqrt((player->x - entityList[j].x) * (player->x - entityList[j].x) + 
						(player->y - entityList[j].y) * (player->y - entityList[j].y))) / (3.141592 / 180.0);

					angleBetweenVerts -= player->pitch;
					rotate_point(player, (entityList + j), player->yaw);





					float yFOV = FOV / width * height;
					float distanceToScrX = (width / 2) / tan((FOV / 2) * (3.141592 / 180.0));
					float distanceToScrY = (height / 2) / tan((yFOV / 2) * (3.141592 / 180.0));

					float xLength = tan(angleBetweenUs * (3.141592 / 180.0)) * distanceToScrX;
					float yLength = tan(angleBetweenVerts * (3.141592 / 180.0)) * distanceToScrY;
					
					
					float distToScreen = (width / 2) * tan(angleToRad(FOV / 2));
					
					
					float enJProjX = tan(angleToRad(angleBetweenUs)) * distanceToScrX;
					float enJProjY = tan(angleToRad(angleBetweenVerts)) * distanceToScrY;
					std::cout << "Vertiacal angle : " << angleBetweenVerts <<
						"\nHorizontal angle : " << angleBetweenUs << "\nName : " << entityList[j].name << "\n\n";
					

					line(width / 2, 0, width / 2 + entityList[j].x - player->x, height / 2, D3DCOLOR_ARGB(255, 255, 255, 255));
					line(width / 2, height / 2, width / 2 - enJProjX, height / 2 - enJProjY, D3DCOLOR_ARGB(255, 255, 0, 0));

					if (angleBetweenUs > 90 || angleBetweenUs < -90) {
						xLength = -xLength;
						//yLength -= height * 2;
					}
					float newV = sin(player->pitch * (3.141592 / 180.0)) * 0.5;// sin(45 * (3.141592 / 180.0));
					rotate(entityList[j].x, entityList[j].y, entityList[j].z, player->x, player->y, player->z, -(player->yaw + 0), 0, 0);

					
					
					rectBordered(entityList[j].x - playerSize + radarOffsetX - player->x, entityList[j].y - playerSize + radarOffsetY - player->y,
						entityList[j].x + playerSize + radarOffsetX - player->x, entityList[j].y + playerSize + radarOffsetY - player->y,
						D3DCOLOR_ARGB(255, 255, 0, 0), D3DCOLOR_ARGB(255, 255, 0, 0));

					rotate(entityList[j].x, entityList[j].y, entityList[j].z, player->x, player->y, player->z, (player->yaw + 0), 0, 0);
					
						
					if (entityList[j].team != player->team) {
						
						
						if (!flag) {
							smallestX = angleBetweenUs;
							smallestY = angleBetweenVerts;
							flag = 1;
						}
						smallestY = abs(angleBetweenVerts) < abs(smallestY) ? angleBetweenVerts : smallestY;
						smallestX = abs(angleBetweenUs) < abs(smallestX) ? angleBetweenUs : smallestX;
						
	
						
					}
					//////////// ESP REGION START
				
					

					
				}
			}
		}
	
		float fs = 0;
		//if (smallestX)
		fs = smallestY + player->pitch;
		//std::cout << smallestX << "    " << smallestY << "  " << (-2 < -5) <<  "          \n";
		if (GetAsyncKeyState(VK_XBUTTON1) < 0) {
			
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.localPlayer + offsets::entityPitch), &fs, sizeof(float), 0); 
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.gameBaseAddress + 0x17ED14), &fs, sizeof(float), 0);
			//Sleep(200);
			fs = player->yaw - smallestX;
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.localPlayer + offsets::entityYaw), &fs, sizeof(float), 0);
		}

		if (GetAsyncKeyState(VK_NUMPAD8) < 0) {
			std::cout << "PIIIITCH\n";
			fs = player->pitch + 0.1;
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.localPlayer + offsets::entityPitch), &fs, sizeof(float), 0);
		}

		if (GetAsyncKeyState(VK_NUMPAD5) < 0) {
			std::cout << "PIIIITCH\n";
			fs = player->pitch - 0.1;
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.localPlayer + offsets::entityPitch), &fs, sizeof(float), 0);
		}

		if (GetAsyncKeyState(VK_NUMPAD4) < 0) {
			std::cout << "PIIIITCH\n";
			fs = player->yaw - 0.1 * (width / height);
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.localPlayer + offsets::entityYaw), &fs, sizeof(float), 0);
		}

		if (GetAsyncKeyState(VK_NUMPAD6) < 0) {
			std::cout << "PIIIITCH\n";
			fs = player->yaw + 0.1 * (width / height);
			WriteProcessMemory(mem.pHandle, (LPVOID)(mem.localPlayer + offsets::entityYaw), &fs, sizeof(float), 0);
		}
	}

	d3dDevice->EndScene();
	d3dDevice->PresentEx(0, 0, 0, 0, 0);

	return 0;
}

void D3DXHelper::clear() {
	d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
}


void D3DXHelper::drawText(char* String, int x, int y, D3DCOLOR color) {
	RECT FontPos;
	FontPos.left = x;
	FontPos.top = y;
	d3dFont->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, color);
}

void D3DXHelper::line(int x, int y, int xe, int ye, D3DCOLOR color) {
	array[0] = D3DXVECTOR2(x, y);
	array[1] = D3DXVECTOR2(xe, ye);
	if (!d3dLine) {
		D3DXCreateLine(d3dDevice, &d3dLine);
	}
	d3dLine->SetWidth(2);
	d3dLine->Draw(array, 2, color);
}

void D3DXHelper::rect(int x, int y, int xe, int ye, D3DCOLOR color) {
	BarRect = { x, y,xe, ye };
	d3dDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void D3DXHelper::rectBordered(int x, int y, int xe, int ye, D3DCOLOR color, D3DCOLOR borderColor) {
	BarRect = { x, y,xe, ye };
	d3dDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);

	line(x, y, xe, y, borderColor);
	line(xe, y, xe, ye, borderColor);
	line(xe, ye, x, ye, borderColor);
	line(x, ye, x, y, borderColor);
}
