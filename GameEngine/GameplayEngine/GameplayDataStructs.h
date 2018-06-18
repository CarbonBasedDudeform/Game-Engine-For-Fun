#pragma once
#include <chrono>

namespace Gameplay {
	using GameClock = std::chrono::high_resolution_clock;
	using GameTimeDelta = std::chrono::time_point<GameClock>;
}