#pragma once
#include "Scope.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Signature struct - used to describe prescribed members for the type manager
	/// </summary>
	struct Signature
	{
		bool operator==(const Signature& other) const
		{
			return (name == other.name && type == other.type && size == other.size);
		}

		bool operator!=(const Signature& other) const
		{
			return !operator==(other);
		}

		/// <summary>
		/// Name of the prescribed member
		/// </summary>
		string name;

		/// <summary>
		/// type of the prescribed member
		/// </summary>
		Datum::DatumType type;

		/// <summary>
		/// Number of elements in the prescribed member
		/// </summary>
		size_t size;

		/// <summary>
		/// Offset of the prescribed member (&class + member location)
		/// </summary>
		size_t offset;
	};

	/// <summary>
	/// Singleton TypeManager class - used to store a list of associated types and their signature arrays for proper RTTI.
	/// </summary>
	class TypeManager
	{
	public:
		/// <summary>
		/// Constructor - deleted to prevent instantiation.
		/// </summary>
		TypeManager() = delete;
		
		/// <summary>
		/// Copy constructor - deleted to prevent instantiation.
		/// </summary>
		TypeManager(const TypeManager& other) = delete;

		/// <summary>
		/// Move constructor - deleted to prevent instantiation.
		/// </summary>
		TypeManager(TypeManager&& other) = delete;

		/// <summary>
		/// Copy Assignment operator - deleted to prevent instantiation.
		/// </summary>
		TypeManager& operator=(const TypeManager& other) = delete;

		/// <summary>
		/// Move Assignment operator - deleted to prevent instantiation.
		/// </summary>
		TypeManager& operator=(TypeManager&& other) = delete;

		/// <summary>
		/// Default destructor.
		/// </summary>
		~TypeManager() = default;

		/// <summary>
		/// ContainsType - Returns a boolean indicating if the passed in typeID is present in the Type manager.
		/// </summary>
		/// <param name="typeID">Type ID of the class that you are checking for definition in the type manager.</param>
		/// <returns>True if the typeID is present in the Type Manager, otherwise false.</returns>
		static bool ContainsType(RTTI::IdType typeID);

		/// <summary>
		/// GetSignaturesForType - Returns the signature array for a given typeID.
		/// </summary>
		/// <param name="typeID">The type ID of the class whose signature array you want to retrieve.</param>
		/// <returns>A reference to the signature array.</returns>
		static const Vector<Signature>& GetSignaturesForType(RTTI::IdType typeID);

		/// <summary>
		/// AddType - Registers a typeID and it's signature array into the type manager
		/// </summary>
		/// <param name="idType">The type ID of the class you are attempting to register.</param>
		/// <param name="signatureVector">The signature array of the class you are attempting to register.</param>
		static void AddType(RTTI::IdType idType, Vector<Signature> signatureVector);

		/// <summary>
		/// RemoveType - Removes the type associated with the typeID passed into it from the Type Manager.
		/// </summary>
		/// <param name="idType">The type ID of the class that you wish to remove from the Type Manager.</param>
		static void RemoveType(RTTI::IdType idType);

		/// <summary>
		/// Clear - Removes all registered types from the Type Manager.
		/// </summary>
		static void Clear();

		/// <summary>
		/// Size - Returns the number of types registered in the type manager.
		/// </summary>
		/// <returns>size_t - The number of unique types registered</returns>
		static size_t Size();

	private:

		/// <summary>
		/// A Hashmap storing (IDType, Signature Array) pairs.
		/// </summary>
		static inline HashMap<RTTI::IdType, Vector<Signature>> _signatureMap;
	};
}