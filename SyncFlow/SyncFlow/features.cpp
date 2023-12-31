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
		const auto armorAddress = localPlayer + offsets::m_ArmorValue;
		const auto mtpAddress = localPlayer + offsets::m_mtp57Ammo;

		if (!localPlayer)
			continue;

		if (globals::godmode)
		{
			memory.Write<int>(healthAddress, 1337);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (globals::armor)
		{
			memory.Write<int>(armorAddress, 1337);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (globals::ammo)
		{
			memory.Write<int>(mtpAddress, 1337);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		
	}
}