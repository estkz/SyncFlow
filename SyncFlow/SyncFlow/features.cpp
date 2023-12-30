#include "features.h"
#include "../menu/gui.h"
#include "../memory/globals.h"

#include <thread>

void features::AimbotThread(const Memory& memory) noexcept
{

}

void features::VisualsThread(const Memory& memory) noexcept
{

}

void features::TrainerThread(const Memory& memory) noexcept
{
	while (gui::isActive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = memory.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);

		const auto healthAddress = localPlayer + offsets::m_iHealth;

		if (!localPlayer)
			continue;

		if (globals::godmodeCB)
		{
			memory.Write<int>(healthAddress, 1337);
		}
	}
}