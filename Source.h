#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <Windows.h>
#include <Psapi.h>
#include <tchar.h> // _tcscmp
#include<TlHelp32.h>
//#include <objidl.h>
//#include <gdiplus.h>
//#pragma comment (lib,"Gdiplus.lib")

namespace gl {
	HWND window {};
	HWND game {};
	RECT gameRect {};
	MSG msg {};
	DWORD pID = 0;
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {


	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}


void setConPos(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ShowConsoleCursor(bool showFlag) {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}


void initWindow(HINSTANCE currentInstance) {
	// Register the window class
	LPCWSTR CLASS_NAME = L"myWin32WindowClass";
	WNDCLASS wcex{};
	wcex.hInstance = currentInstance;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpszClassName = CLASS_NAME;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	wcex.lpfnWndProc = WindowProcessMessages;
	RegisterClass(&wcex);

	gl::game = FindWindowA(0, "AssaultCube");
	if (!gl::game) {
		std::cout << "Couldn`t find window\n";
		exit(1);
	}
	GetWindowRect(gl::game, &gl::gameRect); 

	// Create the window
	gl::window = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, wcex.lpszClassName,
		TEXT("CHEAT ENABLED"), WS_VISIBLE, gl::gameRect.left, gl::gameRect.top, 
		gl::gameRect.right - gl::gameRect.left, gl::gameRect.bottom - gl::gameRect.top, NULL,
		NULL, currentInstance, NULL);
	SetLayeredWindowAttributes(gl::window, RGB(0, 0, 0), 0, ULW_COLORKEY);
	
	MoveWindow(gl::window, gl::gameRect.left, gl::gameRect.top,
		       gl::gameRect.right - gl::gameRect.left, gl::gameRect.bottom - gl::gameRect.top, 0);

	ShowConsoleCursor(0);
}
