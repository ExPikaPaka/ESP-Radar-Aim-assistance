#include "Source.h"
#include "Memory.h"
#include "Entity.h"
#include "D3DXHelper.h"

Entity player;
Entity entity[32];



int main(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
	initWindow(currentInstance);

	Memory memory;
	memory.initMemory();
	memory.initEntityList(entity, 32);
	
	bool gameMode = 0;

	D3DXHelper scene(gl::window, gl::game, gl::gameRect.right - gl::gameRect.left, gl::gameRect.bottom - gl::gameRect.top);
	float fov = memory.readMemory<float>(memory.gameBaseAddress + offsets::localFOV, sizeof(float));
	while (GetAsyncKeyState(VK_RSHIFT) == 0) {
		
		if (PeekMessage(&gl::msg, NULL, 0, 0, PM_REMOVE)) {
			if (gl::msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&gl::msg);
				DispatchMessage(&gl::msg);
			}
		}
		
		memory.readLocalPlayer(&player, gameMode);
		memory.readEntityList(entity, 32);
		fov = memory.readMemory<float>(memory.gameBaseAddress + offsets::localFOV, sizeof(float));

		scene.clear();
		scene.render(&player, entity, 32, memory, fov);
		setConPos(0, 0);

		if (GetAsyncKeyState(VK_NUMPAD0) < 0) {
			memory.readLocalPlayer(&player, gameMode);
			memory.initEntityList(entity, 32);
		}
		if (GetAsyncKeyState(VK_NUMPAD1) < 0) {
			memory.readLocalPlayer(&player, gameMode);
			memory.initEntityList(entity, 32);
		}
	}
	return 0;
}
