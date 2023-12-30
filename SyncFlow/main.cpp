// GUI Function Includes
#include "menu/gui.h"
#include "../SyncFlow/SyncFlow/features.h"
#include "memory/globals.h"

// Miscellaneous Related Includes
#include <thread>

// Main Entrypoint
int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{
	Memory memory{ "ac_client.exe" };

	globals::clientAddress = memory.GetModuleAddress("ac_client.exe");

	std::thread(features::TrainerThread, memory).detach();

	gui::CreateHWindow("SyncFlow");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isActive)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // This line significantly reduces CPU usage; removing it is not recommended.
	}

	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	// Once EXIT_SUCCES gets returned it indicates that the cheat has executed successfully and is now terminating (shutting down).
	return EXIT_SUCCESS;
}