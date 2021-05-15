#include "pch.h"
#include "Attributed.h"

namespace FieaGameEngine 
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(RTTI::IdType typeID)
	{
		(*this)["this"] = this;
		Populate(typeID);
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::move(other))
	{
		(*this)["this"] = this;
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (this != &other)
		{
			Scope::operator=(other);
			(*this)["this"] = this;
			UpdateExternalStorage(other.TypeIdInstance());
		}
		
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));
			(*this)["this"] = this;
			UpdateExternalStorage(other.TypeIdInstance());
		}
		return *this;
	}

	Vector<Signature> Attributed::Attributes() const
	{
		Vector<Signature> newVector;
		newVector.Reserve(_orderList.Size());
		for (size_t i = 0; i < Size(); ++i)
		{
			auto& [key, datum] = *(_orderList[i]);
			Signature entry = { key, datum.Type(), datum.Size(), i };
			newVector.PushBack(entry);
		}

		return newVector;
	}

	Vector<Signature> Attributed::AuxiliaryAttributes() const
	{
		Vector<Signature> newVector;
		for (size_t i = 0; i < Size(); ++i)
		{
			auto& [key, datum] = *(_orderList[i]);
			if (!IsPrescribedAttribute(key))
			{
				Signature entry = { key, datum.Type(), datum.Size(), i };
				newVector.PushBack(entry);
			}
		}

		return newVector;
	}

	const Vector<Signature>& Attributed::PrescribedAttributes() const
	{
		return TypeManager::GetSignaturesForType(TypeIdInstance());
	}

	bool Attributed::IsAttribute(const string& name) const
	{
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const string& name) const
	{
		Vector<Signature> signatures = TypeManager::GetSignaturesForType(TypeIdInstance());
		for (const Signature& signature : signatures)
		{
			if (signature.name == name)
			{
				return true;
			}
		}
		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const string& name) const
	{
		if (IsAttribute(name))
		{
			if (!IsPrescribedAttribute(name))
			{
				return true;
			}
		}
		return false;
	}

	void Attributed::Populate(RTTI::IdType typeID)
	{
		const auto signatures = TypeManager::GetSignaturesForType(typeID);
		for (const auto& signature : signatures)
		{
			Datum& datum = Append(signature.name);
			if (signature.type == Datum::DatumType::Table)
			{
				datum.SetType(Datum::DatumType::Table);
				if (signature.size > 0)
				{
					datum.Reserve(signature.size);
					for (size_t i = 0; i < signature.size; ++i)
					{
						AppendScope(signature.name);
					}
				}
			}
			else
			{
				void* data = reinterpret_cast<uint8_t*>(this) + signature.offset;
				datum.SetStorage(signature.type, data, signature.size);
			}
		}
	}

	void Attributed::UpdateExternalStorage(RTTI::IdType typeID)
	{
		const Vector<Signature> signatures = TypeManager::GetSignaturesForType(typeID);
		size_t prescribedAttributeCount = signatures.Size() + 1; // +1 for "this"

		for (size_t i = 1; i < prescribedAttributeCount; ++i)
		{
			const Signature& signature = signatures[i - 1];
			auto& [key, datum] = *(_orderList[i]);

#if defined(NDEBUG)
			UNREFERENCED_LOCAL(key);
#endif
			
			if (signature.type != Datum::DatumType::Table)
			{
				//	All scopes should be internal data
				assert(!datum._ownsData);
				assert(signature.size == datum.Size());

				void* data = reinterpret_cast<uint8_t*>(this) + signature.offset;
				datum.SetStorage(signature.type, data, signature.size);
			}
		}
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw runtime_error("Attempting to AppendAuxiliaryAttribute with the same name as a prescribed attribute already present in Attributed().");
		}

		return Append(name);
	}

	/*gsl::owner<Attributed*> Attributed::Clone() const
	{
		return new Attributed(*this);
	}*/
}