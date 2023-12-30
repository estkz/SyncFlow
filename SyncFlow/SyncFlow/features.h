#pragma once
#include "../memory/memory.h"

namespace features
{
	void AimbotThread(const Memory& memory) noexcept;
	void VisualsThread(const Memory& memory) noexcept;
	void TrainerThread(const Memory& memory) noexcept;
}
