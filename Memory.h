#pragma once
#include <Windows.h>
#include <iostream>
#include<TlHelp32.h>
#include <tchar.h> // _tcscmp
#include "Entity.h"

namespace offsets {
	static int playerBase = 0x0018AC00;
	static int entityList = 0x00191FCC;
	static int entityListM = 0x0018AC04;
	static int entityYaw = 0x34;
	static int entityPitch = 0x38;
	static int testValue = 0x28;

	static int localPitch = 0x17ED14;
	static int localFOV = 0x18A7CC;

	static int x = 0x4;
	static int y = 0x8;
	static int z = 0xc;

	static int name = 0x205;
	static int hp = 0xEC;

	static int team = 0x32C;
	static int teamOffline = 0x30C;
}
//D12BA68 DieHard blue
//D12B620 fried blue
//E001C90 Supa red
class Memory {
public: 
	HWND game {};
	DWORD pID {};
	HANDLE pHandle{};

	int entityAddressPointer{};
	int entityPointer[32]{};
	int gameBaseAddress{};
	int localPlayer{};
	int playerCnt{};
	int entityIdentifier{};
	int buff{};
	
	void initMemory();
	void initEntityList(Entity* entity, int cnt);
	
	int getModuleBaseAddress(TCHAR* lpszModuleName, DWORD procID);
	template<typename T> T readMemory(int address, int32_t amountOfBytesToRead);
	template<typename T> bool writeMemory(int address, T value);

	void readEntity(Entity *entity, int entityPtr);
	void readLocalPlayer(Entity* entity, bool isOnline);
	void readEntityList(Entity* entity, int cnt);
};

