#pragma once
#include "json/value.h"
#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	class SharedData;
	class JsonParseCoordinator;

	/// <summary>
	/// IJsonParseHelper - Interface for ParseHelpers that accept different grammars used in parsing Json files. Abstract Class - can't instantiate.
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)

		friend JsonParseCoordinator;
	public:
		
		/// <summary>
		/// Defaulted Constructor.
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// Defaulted Copy Constructor
		/// </summary>
		/// <param name="other">Thing being Copied.</param>
		IJsonParseHelper(const IJsonParseHelper& other) = default;

		/// <summary>
		/// Defaulted Move constructor
		/// </summary>
		/// <param name="other">Thing being moved.</param>
		IJsonParseHelper(IJsonParseHelper&& other) = default;

		/// <summary>
		/// Defaulted copy assignment operator.
		/// </summary>
		/// <param name="other">Thing being copied.</param>
		/// <returns>Reference to the copy.</returns>
		IJsonParseHelper& operator=(const IJsonParseHelper& other) = default;

		/// <summary>
		/// Defaulted move assignment operator.
		/// </summary>
		/// <param name="other">Thing being moved.</param>
		/// <returns>Reference to the moved object.</returns>
		IJsonParseHelper& operator=(IJsonParseHelper&& other) = default;

		/// <summary>
		/// Virtual Destructor - defaulted. Necessary to put into V-table since at least one virtual method.
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Initialize Method - Does nothing in the interface, but available if necessary for implementing classes.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Cleanup Method - Does nothing in the interface, but available if necessary for implementing classes.
		/// </summary>
		virtual void CleanUp();

		/// <summary>
		/// Start Handler - Pure virtual method. Implementation expected to determine if the grammar of a json value object can be deciphered, return true if so,
		/// false if not, - and if so also handle the parsing of the json data and stores it into the SharedData passed into it.
		/// </summary>
		/// <param name="data">Output Parameter where the parsed data handled by the helper is stored.</param>
		/// <param name="key">Key for the json value/object being passed in.</param>
		/// <param name="object">The json value/object to be parsed.</param>
		/// <param name="inArray">Boolean indicating if the json object is an array.</param>
		/// <returns>True if the data can - and was - handled, false if the helper cannot handle grammar for this data type.</returns>
		virtual bool StartHandler(SharedData& data, const std::string& key, const Json::Value& object, bool inArray, size_t index) = 0;

		/// <summary>
		/// End Handler - Pure virtual method.  Implementation expected to determine if the grammar of a json value object was able to be deciphered, return true if so,
		/// elsewise return false. Performs any cleanup necessary for the handler.
		/// </summary>
		/// <param name="data">The output parameter data where the parsed information is stored.</param>
		/// <param name="key">The last key that the handler handled.</param>
		/// <returns>True if the handler was able to handle the information passed into start handler, false otherwise.</returns>
		virtual bool EndHandler(SharedData& data, const std::string& key) = 0;

		/// <summary>
		/// Create - Emulation of a virtual constructor utilized within the Clone method of the parse master. This needs to be pointed correctly at implementing
		/// data types.
		/// </summary>
		/// <returns>A pointer to the address of a newly instantiated object of *this' data type.</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() = 0;
	};


}