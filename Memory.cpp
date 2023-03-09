#include "Memory.h"

void Memory::initMemory() {
	game = FindWindowA(0, "AssaultCube");
	if (!game) {
		std::cout << "Couldn`t find window!\n";
		exit(1);
	}
	pID = 0;
	GetWindowThreadProcessId(game, &pID);
	
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pID);
	if (pHandle == INVALID_HANDLE_VALUE || pHandle == NULL) { // error handling
		std::cout << "Failed to open process handle" << '\n';
		exit(2);
	}

	wchar_t gameName[] = L"ac_client.exe";
	gameBaseAddress = getModuleBaseAddress(gameName, pID);
}

void Memory::initEntityList(Entity* entity, int cnt) {
	playerCnt = cnt;
	int entityListPointer = readMemory<int>(gameBaseAddress + offsets::entityList, sizeof(int));

	int testEntityPointer = readMemory<int>(entityListPointer, sizeof(int));
	int testSmthPointer = readMemory<int>(testEntityPointer, sizeof(int));
	char testValue = readMemory<char>(testSmthPointer + offsets::testValue, sizeof(char));

	if (testValue != 12) {
		entityListPointer = readMemory<int>(gameBaseAddress + offsets::entityListM, sizeof(int));
	}
	for (int i = 0; i < cnt; i++) {
		entityPointer[i] = readMemory<int>(entityListPointer, sizeof(int));
		readEntity(entity + i, entityPointer[i]);
		entityListPointer += 0x4;
	}
}



int Memory::getModuleBaseAddress(TCHAR* lpszModuleName, DWORD procID) {
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, procID); // make snapshot of all modules within process
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32)) {  //store first Module in ModuleEntry32
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) {   // if Found Module matches Module we look for -> done!
				dwModuleBaseAddress = (int)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32
	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}


template<typename T> 
inline T Memory::readMemory(int address, int32_t amountOfBytesToRead) {
	T buffer {};
	ReadProcessMemory(pHandle, (LPVOID)address, &buffer, amountOfBytesToRead, 0);
	
	return buffer;
}


template<typename T>
bool Memory::writeMemory(int address, T value) {
	T buff = value;
	return WriteProcessMemory(pHandle, (LPVOID)address, &buff, sizeof(buff), 0);
}


void Memory::readEntity(Entity *entity, int entityPtr) {
	if (!entityIdentifier) {
		entityIdentifier = readMemory<int>(entityPtr, sizeof(float));
	}
	if (entityIdentifier) {
		buff = readMemory<int>(entityPtr, sizeof(float));
		if (buff != entityIdentifier) {
			entity->isGood = 0;
		}
		else {
			entity->isGood = 1;
		}
	}
	entity->x = readMemory<float>(entityPtr + offsets::x, sizeof(float));
	entity->y = readMemory<float>(entityPtr + offsets::y, sizeof(float));
	entity->z = readMemory<float>(entityPtr + offsets::z, sizeof(float));

	entity->hp = readMemory<int>(entityPtr + offsets::hp, sizeof(int));

	entity->team = readMemory<bool>(entityPtr + offsets::team, sizeof(bool));
	if ((entity->team != 0) || (entity->team != 1)) {
		entity->team = readMemory<bool>(entityPtr + offsets::teamOffline, sizeof(bool));
	}
	entity->pitch = readMemory<float>(entityPtr + offsets::entityPitch, sizeof(float));
	entity->yaw = readMemory<float>(entityPtr + offsets::entityYaw, sizeof(float));

	ReadProcessMemory(pHandle, (LPCVOID)(entityPtr + offsets::name), entity->name, sizeof(entity->name), 0);
}


void Memory::readLocalPlayer(Entity* entity, bool isOnline) {
	localPlayer = readMemory<int>(gameBaseAddress + offsets::playerBase, sizeof(int));

	entity->x = readMemory<float>(localPlayer + offsets::x, sizeof(float));
	entity->y = readMemory<float>(localPlayer + offsets::y, sizeof(float));
	entity->z = readMemory<float>(localPlayer + offsets::z, sizeof(float));

	entity->hp = readMemory<int>(localPlayer + offsets::hp, sizeof(int));

	entity->team = readMemory<bool>(localPlayer + offsets::teamOffline, sizeof(bool));
	if (isOnline) {
		entity->team = readMemory<bool>(localPlayer + offsets::team, sizeof(bool));
	}

	entity->pitch = readMemory<float>(localPlayer + offsets::entityPitch, sizeof(float));
	entity->yaw = readMemory<float>(localPlayer + offsets::entityYaw, sizeof(float));

	entity->isGood = 1;
	ReadProcessMemory(pHandle, (LPCVOID)(localPlayer + offsets::name), entity->name, sizeof(entity->name), 0);
}

void Memory::readEntityList(Entity* entity, int cnt) {
	for (int i = 0; i < cnt; i++) {
		readEntity(entity + i, entityPointer[i]);
	}
}





