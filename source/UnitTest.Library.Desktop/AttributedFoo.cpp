#include "pch.h"
#include "AttributedFoo.h"
#include <exception>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(AttributedFoo)

#pragma region Rule_Of_Six
		//	Constructor
	AttributedFoo::AttributedFoo() :
		Attributed(TypeIdClass())
	{
	}

#pragma endregion

	const Vector<Signature> AttributedFoo::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Integer", Datum::DatumType::Integer, 1, offsetof(AttributedFoo, Integer)},
			{"Float", Datum::DatumType::Float, 1, offsetof(AttributedFoo, Float)},
			{"Vector", Datum::DatumType::Vector, 1, offsetof(AttributedFoo, Vector)},
			{"Matrix", Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, Matrix)},
			{"String", Datum::DatumType::String, 1, offsetof(AttributedFoo, String)},
			{"scope", Datum::DatumType::Table, 1, offsetof(AttributedFoo, scope)},
			{"Rtti", Datum::DatumType::Pointer, 1, offsetof(AttributedFoo, Rtti)},
			{"IntegerArray", Datum::DatumType::Integer, 2, offsetof(AttributedFoo, IntegerArray)},
			{"FloatArray", Datum::DatumType::Float, 2, offsetof(AttributedFoo, FloatArray)},
			{"VectorArray", Datum::DatumType::Vector, 2, offsetof(AttributedFoo, VectorArray)},
			{"MatrixArray", Datum::DatumType::Matrix, 2, offsetof(AttributedFoo, MatrixArray)},
			{"StringArray", Datum::DatumType::String, 2, offsetof(AttributedFoo, StringArray)},
			{"scopeArray", Datum::DatumType::Table, 2, offsetof(AttributedFoo, scopeArray)},
			{"RttiArray", Datum::DatumType::Pointer, 2, offsetof(AttributedFoo, RttiArray)}
		};

		return signatureVector;
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}
}