#include "pch.h"
#include "GameTime.h"

using namespace std::chrono;

namespace FieaGameEngine
{
	const high_resolution_clock::time_point& GameTime::CurrentTime() const
	{
		return _currentTime;
	}

	void GameTime::SetCurrentTime(const high_resolution_clock::time_point& currentTime)
	{
		_currentTime = currentTime;
	}

	const milliseconds& GameTime::TotalGameTime() const
	{
		return _totalGameTime;
	}

	void GameTime::SetTotalGameTime(const milliseconds& totalGameTime)
	{
		_totalGameTime = totalGameTime;
	}

	const milliseconds& GameTime::ElapsedGameTime() const
	{
		return _elapsedGameTime;
	}

	void GameTime::SetElapsedGameTime(const milliseconds& elapsedGameTime)
	{
		_elapsedGameTime = elapsedGameTime;
	}

	duration<float> GameTime::TotalGameTimeSeconds() const
	{
		return duration_cast<duration<float>>(_totalGameTime);
	}

	duration<float> GameTime::ElapsedGameTimeSeconds() const
	{
		return duration_cast<duration<float>>(_elapsedGameTime);
	}
}