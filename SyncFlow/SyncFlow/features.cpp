#include "features.h"
#include "../menu/gui.h"
#include "../memory/globals.h"

#include <thread>

const int epicHackerModeNumberLOLLLZMLGGGG = 1337;

void features::AimbotThread(const Memory& memory) noexcept
{
	while (gui::isActive)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const auto localPlayer = memory.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
	}
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

		// Trainer Addresses
		const auto healthAddress = localPlayer + offsets::m_iHealth;
		const auto armorAddress = localPlayer + offsets::m_ArmorValue;
		const auto mtpAddress = localPlayer + offsets::m_mtp57Ammo;
		const auto nadeAddress = localPlayer + offsets::m_GrenadeValue;

		const auto iJumpingAddress = localPlayer + offsets::m_iJumping;

		if (!localPlayer)
			continue;

		if (globals::godmode)
		{
			memory.Write<int>(healthAddress, epicHackerModeNumberLOLLLZMLGGGG);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (globals::armor)
		{
			memory.Write<int>(armorAddress, epicHackerModeNumberLOLLLZMLGGGG);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		
		if (globals::ammo)
		{
			memory.Write<int>(mtpAddress, epicHackerModeNumberLOLLLZMLGGGG);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (globals::nades)
		{
			memory.Write<int>(nadeAddress, epicHackerModeNumberLOLLLZMLGGGG);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (globals::bunnyhop)
		{
			int bhopFlag = memory.Read<int>(iJumpingAddress);

			if (!(bhopFlag == 0))
			{
				memory.Write<int>(iJumpingAddress, 0);
			}
		}
		
	}
}