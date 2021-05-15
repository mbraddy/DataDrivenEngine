#pragma once

#include <exception>
#include <chrono>

namespace FieaGameEngine
{
	class GameTime;

	/// <summary>
	/// GameClock - Class that maintains a clock which contains the time that the game is started, and works with GameTime to provide a delta T to be used
	/// within the game engine.
	/// </summary>
	class GameClock final
	{
	public:

		/// <summary>
		/// Constructor - Just calls reset()
		/// </summary>
		GameClock();

		/// <summary>
		/// Copy constructor - defaulted
		/// </summary>
		/// <param name="other">GameClock to be copied.</param>
		GameClock(const GameClock& other) = default;

		/// <summary>
		/// Copy assignment operator - defaulted
		/// </summary>
		/// <param name="other">GameClock to be copied.</param>
		GameClock& operator=(const GameClock& other) = default;

		/// <summary>
		/// Move constructor - defaulted
		/// </summary>
		/// <param name="">GameClock to be moved.</param>
		GameClock(GameClock&& other) = default;

		/// <summary>
		/// Move assignment operator - defaulted
		/// </summary>
		/// <param name="">GameClock to be moved.</param>
		GameClock& operator=(GameClock&& other) = default;

		/// <summary>
		/// Destructor - defaulted
		/// </summary>
		~GameClock() = default;

		/// <summary>
		/// StartTime - getter for the GameClock::_startTime member.
		/// </summary>
		/// <returns>Immutable reference to _startTime</returns>
		const std::chrono::high_resolution_clock::time_point& StartTime() const;

		/// <summary>
		/// CurrentTime - getter for the GameClock::_currentTime member.
		/// </summary>
		/// <returns>Immutable reference to _currentTime</returns>
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;

		/// <summary>
		/// LastTime - getter for the GameClock::_lastTime member.
		/// </summary>
		/// <returns>Immutable reference to _lastTime</returns>
		const std::chrono::high_resolution_clock::time_point& LastTime() const;

		/// <summary>
		/// Reset - Restarts the GameClock and sets the start, current, and last time all to high_resolution_clock::now()
		/// </summary>
		void Reset();

		/// <summary>
		/// UpdateGameTime - Window to the gameclock used by GameTime to update the delta T for the engine.
		/// </summary>
		/// <param name="gameTime">Out param - The reference to the GameTime object that is being updated.</param>
		void UpdateGameTime(GameTime& gameTime);

	private:

		/// <summary>
		/// _startTime - The high_resolution_clock::time_point of the exact point in time that the gameclock was started. 
		/// </summary>
		std::chrono::high_resolution_clock::time_point _startTime;

		/// <summary>
		/// _currentTime - The high_resolution_clock::time_point of the exact point in time that it currently is at the time of invocation. 
		/// </summary>
		std::chrono::high_resolution_clock::time_point _currentTime;

		/// <summary>
		/// _lastTime - The high_resolution_clock::time_point of the exact point in time that the last time UpdateGameTime() was called. 
		/// </summary>
		std::chrono::high_resolution_clock::time_point _lastTime;
	};
}