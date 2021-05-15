#include "pch.h"
#include "Avatar.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Avatar)

	Avatar::Avatar() :
		GameObject(Avatar::TypeIdClass())
	{
	}

	const Vector<Signature> Avatar::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Name", Datum::DatumType::String, 1, offsetof(Avatar, _name)},
			{"Transform", Datum::DatumType::Matrix, 1, offsetof(Avatar, _transform)},
			{"Actions", Datum::DatumType::Table, 0, 0},
			{"Children", Datum::DatumType::Table, 0, 0},
			{"Health", Datum::DatumType::Integer, 1, offsetof(Avatar, _health)},
			{"Velocity", Datum::DatumType::Vector, 1, offsetof(Avatar, _velocity)},
			{"Dps", Datum::DatumType::Float, 1, offsetof(Avatar, _dps)}
		};

		return signatureVector;
	}

	void Avatar::Update(GameState& gameState)
	{
		gameState.SetCurrentGameObject(*this);

		GameObject::Update(gameState);
	}

	void Avatar::SetHealth(int hp)
	{
		_health = hp;
	}

	int Avatar::GetHealth()
	{
		return _health;
	}

	gsl::owner<Avatar*> Avatar::Clone() const
	{
		return new Avatar(*this);
	}
}