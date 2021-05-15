#pragma once

#include <chrono>
#include "GameClock.h"

namespace FieaGameEngine
{
	/// <summary>
	/// GameTime - A wrapper for a time_point that represents the time of the current "frame". Also contains a tracker for the total game time and elapsed game time.
	/// </summary>
	class GameTime final
	{
	public:

		/// <summary>
		/// CurrentTime - Getter for the _currentTime member.
		/// </summary>
		/// <returns>Immutable reference to the _currentTime time_point member.</returns>
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const;

		/// <summary>
		/// SetCurrentTime - Setter for the _currentTime member.
		/// </summary>
		/// <param name="currentTime">time_point value that you wish to set _currentTime to.</param>
		void SetCurrentTime(const std::chrono::high_resolution_clock::time_point& currentTime);
		
		/// <summary>
		/// TotalGameTime - Getter for the _totalGameTime member.
		/// </summary>
		/// <returns>Immutable reference to the total game time</returns>
		const std::chrono::milliseconds& TotalGameTime() const;

		/// <summary>
		/// SetTotalGameTime - Setter for the _totalGameTime
		/// </summary>
		/// <param name="totalGameTime">value to set the _totalGameTime to.</param>
		void SetTotalGameTime(const std::chrono::milliseconds& totalGameTime);

		/// <summary>
		/// ElapsedGameTime - Getter for _elapsedGameTime member.
		/// </summary>
		const std::chrono::milliseconds& ElapsedGameTime() const;

		/// <summary>
		/// SetElapsedGameTime - Setter for _elapsedGameTime member.
		/// </summary>
		/// <param name="elapsedGameTime">value to set _elapsedGameTime to</param>
		void SetElapsedGameTime(const std::chrono::milliseconds& elapsedGameTime);

		/// <summary>
		/// TotalGameTimeSeconds - Getter of the total game time as seconds (represented by a float)
		/// </summary>
		/// <returns>Float value that represents the number of total game time as seconds</returns>
		std::chrono::duration<float> TotalGameTimeSeconds() const;

		/// <summary>
		/// ElapsedGameTimeSeconds - Getter of the elapsed game time as seconds (represented by a float)
		/// </summary>
		/// <returns>Float value that represents the number of elapsed game time as seconds</returns>
		std::chrono::duration<float> ElapsedGameTimeSeconds() const;

	private:

		/// <summary>
		/// _currentTime - time_point value that represents the current frame's time value.
		/// </summary>
		std::chrono::high_resolution_clock::time_point _currentTime;

		/// <summary>
		/// _totalGameTime - number of milliseconds that represents total game time.
		/// </summary>
		std::chrono::milliseconds _totalGameTime{ 0 };

		/// <summary>
		/// _elapsedGameTime - number of milliseconds that represents elapsed game time.
		/// </summary>
		std::chrono::milliseconds _elapsedGameTime{ 0 };
	};
}