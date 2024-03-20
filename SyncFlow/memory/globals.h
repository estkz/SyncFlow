#pragma once
#include <cstdint>
#include <cstddef>

namespace globals
{
	inline std::uintptr_t clientAddress = 0;

	// =-=-=-=-=-=-=-=-= //
	//  Menu Components  //
	// =-=-=-=-=-=-=-=-= //

	// Aimbot - Aimbot Tab
	inline bool aimbotEnabled = false;
	inline float aimFOV = 10.0f;
	inline float aimSmoothing = 10.0f;
	inline bool aimThroughWalls = false;

	// Aimbot - Extra Tab
	inline bool noRecoil = false;
	inline bool noPushback = false;

	// Aimbot - Triggerbot Tab
	inline bool triggerEnabled = false;
	inline int reactionTime = 0;

	// Visuals - Enemies Tab
	inline bool espEnabled = false;
	inline bool boundingBox = false;
	inline bool healthBar = false;
	inline bool ammoBar = false;
	inline bool nameESP = false;
	inline bool distance = false;
	inline bool weapon = false;
	inline bool includeTeam = false;
	inline bool snaplines = false;

	// Visuals - World Tab
	inline bool nadeESP = false;
	inline bool flagESP = false;

	// Trainer - Hacks Tab
	inline bool godmode = false;
	inline bool armor = false;
	inline bool ammo = false;
	inline bool nades = false;

	// Trainer - Misc Tab
	inline bool bunnyhop = false;

	// Trainer - Exploits
	inline bool rapidfire = false;
}

namespace offsets
{
	const auto dwLocalPlayer = 0x10F4F4;
	const auto dwEntityList = 0x10F4F8;

	const auto m_posHeadPositionX = 0x4;
	const auto m_posHeadPositionY = 0x8;
	const auto m_posHeadPositionZ = 0xC;
	const auto m_posPlayerPositionX = 0x34;
	const auto m_posPlayerPositionY = 0x38;
	const auto m_posPlayerPositionZ = 0x3C;

	const auto m_iViewangleX = 0x40;

	const auto m_iHealth = 0xF8;
	const auto m_ArmorValue = 0xFC;

	const auto m_tmpAmmo = 0x140;
	const auto m_v19Ammo = 0x144;
	const auto m_aapdAmmo = 0x148;
	const auto m_ad91Ammo = 0x14C;
	const auto m_mtp57Ammo = 0x150;
	const auto m_GrenadeValue = 0x158;
	const auto m_mk77Ammo = 0x15C;

	const auto m_iJumping = 0x68;
}
