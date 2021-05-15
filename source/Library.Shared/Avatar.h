#pragma once
#include "GameObject.h"


namespace FieaGameEngine
{
	/// <summary>
	/// Avatar - Test Framework for GameObject, inherits and provides additional prescribed members. Primarily for testing.
	/// </summary>
	class Avatar final : public GameObject
	{
		RTTI_DECLARATIONS(Avatar, GameObject)

	public:
		/// <summary>
		/// Default Constructor - Passes in the Avatar TypeIdClass.
		/// </summary>
		Avatar();

		/// <summary>
		/// Copy Constructor - Defaulted.
		/// </summary>
		/// <param name="other">Avatar being copied.</param>
		Avatar(const Avatar& other) = default;

		/// <summary>
		/// Move Constructor - Defaulted.
		/// </summary>
		/// <param name="other">Avatar being moved.</param>
		Avatar(Avatar&& other) = default;

		/// <summary>
		/// Copy Assignment Operator - Defaulted.
		/// </summary>
		/// <param name="other">Avatar being copied.</param>
		/// <returns>Reference to the copied avatar.</returns>
		Avatar& operator=(const Avatar& other) = default;

		/// <summary>
		/// Move Assignment Operator - Defaulted.
		/// </summary>
		/// <param name="other">Avatar being moved.</param>
		/// <returns>Reference to the moved avatar.</returns>
		Avatar& operator=(Avatar&& other) = default;

		/// <summary>
		/// Virtual default destructor - Defaulted
		/// </summary>
		virtual ~Avatar() = default;

		/// <summary>
		/// GetHealth - returns the health member. Used for testing the update function.
		/// </summary>
		/// <returns></returns>
		int GetHealth();

		void SetHealth(int hp);

		/// <summary>
		/// Clone - Returns a copy of the avatar instance.
		/// </summary>
		/// <returns>Reterns pointer to the address of a newly allocated copy. Must be deleted by the caller when finished.</returns>
		virtual gsl::owner<Avatar*> Clone() const override;

		/// <summary>
		/// Signatures - Returns the vector of prescribed attributes for registration in the TypeManager.
		/// </summary>
		/// <returns>Vector of Signatures for this class.</returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Update - Called by the hierarchy in order to update the game object on tick. Currently just sets the health to a different value for testing.
		/// </summary>
		/// <param name="gameState">The current Gamestate (containing the game clock)</param>
		virtual void Update(GameState& gameState) override;

	private:
		/// <summary>
		/// _health - Private health member. Currently used to test the update function.
		/// </summary>
		int _health = 100;

		/// <summary>
		/// _velocity - Private velocity member. Currently used for code coverage on external data.
		/// </summary>
		vec4 _velocity = {0.0, 0.0, 0.0, 0.0};

		/// <summary>
		/// _dps - Private dps member. Currently used for code coverage on external data.
		/// </summary>
		float _dps = 0.0;
	};

}
