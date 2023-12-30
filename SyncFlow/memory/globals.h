#pragma once
#include <cstdint>
#include <cstddef>

namespace globals
{
	inline std::uintptr_t clientAddress = 0;

	inline bool godmodeCB = false;
}

namespace offsets
{
	const auto dwLocalPlayer = 0x10F4F4;
	const auto dwEntityList = 0x4; // unknown

	const auto m_iHealth = 0xF8;
}
