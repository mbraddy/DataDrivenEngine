#include "pch.h"
#include "TypeManager.h"


namespace FieaGameEngine
{

	bool TypeManager::ContainsType(RTTI::IdType typeID)
	{
		return _signatureMap.ContainsKey(typeID);
	}

	const Vector<Signature>& TypeManager::GetSignaturesForType(RTTI::IdType typeID)
	{
		return _signatureMap.At(typeID);
	}

	void TypeManager::AddType(RTTI::IdType idType, Vector<Signature> signatureVector)
	{
		_signatureMap.Insert(make_pair(idType, std::move(signatureVector)));
	}

	void TypeManager::RemoveType(RTTI::IdType idType)
	{
		_signatureMap.Remove(idType);
	}

	size_t TypeManager::Size()
	{
		return _signatureMap.Size();
	}

	void TypeManager::Clear()
	{
		_signatureMap.Clear();
	}
}
