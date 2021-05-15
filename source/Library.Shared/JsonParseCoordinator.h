#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "IJsonParseHelper.h"
#include "Stack.h"

namespace FieaGameEngine
{
	class IJsonParseHelper;
	class JsonParseCoordinator;

	/// <summary>
	/// Shared Data - Abstract Class - Interface for Shared Data types that can inherit from RTTI in order to
	/// determine run time typing and determine whether or not any JsonParseHelpers can parse the grammar of a json file.
	/// </summary>
	class SharedData : public RTTI
	{
		RTTI_DECLARATIONS(SharedData, RTTI)

		friend JsonParseCoordinator;

	public:
		/// <summary>
		/// Defaulted Default Constructor
		/// </summary>
		SharedData() = default;

		/// <summary>
		/// Defaulted Copy Constructor.
		/// </summary>
		/// <param name="other">Thing being copied.</param>
		SharedData(const SharedData& other) = default;

		/// <summary>
		/// Defaulted Move Constructor.
		/// </summary>
		/// <param name="other">Thing being moved.</param>
		SharedData(SharedData&& other) = default;

		/// <summary>
		/// Defaulted copy assignment operator.
		/// </summary>
		/// <param name="other">Thing being copied.</param>
		/// <returns>Reference to the copy.</returns>
		SharedData& operator=(const SharedData& other) = default;

		/// <summary>
		/// Defaulted move assignment operator.
		/// </summary>
		/// <param name="other">Thing being moved.</param>
		/// <returns>Reference to the moved object.</returns>
		SharedData& operator=(SharedData&& other) = default;

		/// <summary>
		/// Virtual defaulted destructor - necessary for the v-table due to having at least one virtual method.
		/// </summary>
		virtual ~SharedData() = default;

		/// <summary>
		/// GetJsonParseCoordinator - Returns the address of the parse coordinator associated with this shared data object.
		/// </summary>
		/// <returns></returns>
		JsonParseCoordinator* const GetJsonParseCoordinator() const;

		/// <summary>
		/// Depth - Returns the current depth level as a size_t.
		/// </summary>
		/// <returns>Current depth of nesting as a size_t.</returns>
		size_t Depth() const;

		/// <summary>
		/// SharedData::Initialize() - Virtual Initialize method, For base SharedData just sets depth to 0.
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// SharedData::CleanUp() - Virtual Cleanup method, does nothing for base SharedData.
		/// </summary>
		virtual void CleanUp();

	protected:

		/// <summary>
		/// Create - Emulation of a virtual constructor which allows proper instantiation of a new shared data object at runtime.
		/// Pure virtual - must be implemented.
		/// </summary>
		/// <returns>A pointer to the newly instantiated type of shared data created by implementing children.</returns>
		virtual gsl::owner<SharedData*> Create() = 0;

	private:

		/// <summary>
		/// SetJsonParseCoordinator - Associates a given parse coordinator address with the shared data.
		/// </summary>
		/// <param name="parseMasterAddress">The address of the parse coordinator that this shared data will be associated with.</param>
		void SetJsonParseCoordinator(JsonParseCoordinator* parseMasterAddress);

		/// <summary>
		/// DecrementDepth - Decrements _depth.
		/// </summary>
		void DecrementDepth();

		/// <summary>
		/// IncrementDepth - Increments _depth.
		/// </summary>
		void IncrementDepth();

		/// <summary>
		/// _depth - The level of nesting in the json parsing.
		/// </summary>
		size_t _depth = 0;

		/// <summary>
		/// _parseMaster - The address of the parse coordinator which is associated with this shared data.
		/// </summary>
		JsonParseCoordinator* _parseMaster = nullptr;
	};

	/// <summary>
	/// JsonParseCoordinator - The parse master - Contains a list of helpers which provide a way to parse the grammar of Json files into c++.
	/// </summary>
	class JsonParseCoordinator
	{

	public:
		/// <summary>
		/// Default Constructor - Deleted to prevent instantiation without assigning a shared data.
		/// </summary>
		JsonParseCoordinator() = delete;

		/// <summary>
		/// Copy Constructor - Deleted to prevent improper memory handling when making copies of the coordinator. If a copy is needed, call Clone().
		/// </summary>
		/// <param name="other">Thing being copied.</param>
		JsonParseCoordinator(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Copy Assignment Operator - Deleted to prevent improper memory handling when making copies of the coordinator. If a copy is needed, call Clone().
		/// </summary>
		/// <param name="other">Thing being copied.</param>
		/// <returns>Reference to the copy.</returns>
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Constructor - Proper constructor to call with the parse coordinator. Assigns the passed in shared data as it's shared data member.
		/// </summary>
		/// <param name="data">The shared data that will be used as an output parameter during parsing of json files.</param>
		JsonParseCoordinator(SharedData& data);

		/// <summary>
		/// Move constructor - Provides a way to std::move JsonParseCoordinators.
		/// </summary>
		/// <param name="other">Coordinator being moved.</param>
		/// <returns>Reference to the moved coordinator.</returns>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Move Assignment Operator - Provides a way to std::move JsonParseCoordinators.
		/// </summary>
		/// <param name="other">The Coordinator being moved.</param>
		/// <returns>Reference to the moved coordinator.</returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Destructor - Could not be defaulted since it needs to clean up after cloned Coordinators.
		/// </summary>
		~JsonParseCoordinator();

		/// <summary>
		/// Initialize - Loops through all helpers and the shared data and calls their Initialize methods.
		/// </summary>
		void Initialize();

		/// <summary>
		/// CleanUp - Loops through all helpers and the shared data and calls their CleanUp methods.
		/// </summary>
		void CleanUp();

		/// <summary>
		/// Clear - Deletes all helpers and the shared data.
		/// </summary>
		void Clear();

		/// <summary>
		/// Clone - Instantiates a clone of the coordinator. Creates copies of all helpers and the shared data.
		/// Must be deleted by the user when done otherwise memory leaks.
		/// </summary>
		/// <returns>Address of the newly created clone.</returns>
		gsl::owner<JsonParseCoordinator*> Clone();

		/// <summary>
		/// AddHelper - Adds a helper that implements the Interface IJsonParseHelper. It is expected that these helpers can handle specific types of grammar
		/// in regards to parsing Json files.
		/// </summary>
		/// <param name="helper">L-value reference to the helper being added to the helper list.</param>
		void AddHelper(const IJsonParseHelper& helper);

		/// <summary>
		/// RemoveHelper - Removes a helper from the helper list. For a description on the helper either see IJsonParseHelper.h or AddHelper()
		/// </summary>
		/// <param name="helper">The helper being removed from the helper list.</param>
		void RemoveHelper(const IJsonParseHelper& helper);

		/// <summary>
		/// GetHelperCount - Returns the total number of helpers in the helper list - Primarily used for unit testing.
		/// </summary>
		/// <returns>size_t number of helpers in the helper list.</returns>
		size_t GetHelperCount() const;

		/// <summary>
		/// Parse - Converts a string into an istream and passes it into Parse(istream).
		/// </summary>
		/// <param name="jsonString">The Json string to be parsed.</param>
		void Parse(std::string& jsonString);

		/// <summary>
		/// Parse - Utilizes JsonCpp to parse an istream of json. The JSon::Value created is passed into the helper function ParseMembers(Json::Value).
		/// </summary>
		/// <param name="jsonStream">Stream of json data to be parsed.</param>
		void Parse(std::istream& jsonStream);

		/// <summary>
		/// ParseFromFile - Opens the filename passed in with an ifstream and passes that stream into Parse(istream)
		/// </summary>
		/// <param name="fileName">Json File to be parsed.</param>
		void ParseFromFile(std::string& fileName);

		/// <summary>
		/// GetFileName - returns the filename of the last file parsed by the ParseCoordinator
		/// </summary>
		/// <returns>A string containing the file name of the last file to be parsed by the coordinator.</returns>
		const std::string& GetFileName();

		/// <summary>
		/// GetSharedData - Returns the address of the shared data member associated with this parse coordinator.
		/// </summary>
		/// <returns>Address of the shared data member associated with this parse coordinator.</returns>
		SharedData* GetSharedData();

		/// <summary>
		/// SetSharedData - Updates the shared data member of this parse coordinator to the passed in shared data value.
		/// </summary>
		/// <param name="sharedData">The shared data you are associating with this coordinator.</param>
		void SetSharedData(SharedData& sharedData);

	private:
		/// <summary>
		/// ParseMembers - Iterates through the members of the Json root and passes each one into calls of the parse helper Parse(key, json::value).
		/// </summary>
		/// <param name="root">Root json value containing multiple members to be parsed.</param>
		void ParseMembers(const Json::Value& root, bool isArrayElement = false, size_t index = 0);

		/// <summary>
		/// Parse - Helper function that loops through all the helpers in the helper list and if the sharedData
		/// type matches that of the helper (signifiying it can handle the grammar) - then the data is parsed from the Json::Value's into the sharedData
		/// via the Helper->StartHandler() call.
		/// </summary>
		/// <param name="key">Name of the current Json::Value attribute.</param>
		/// <param name="value">The Json Value containing the information to be parsed.</param>
		void Parse(const std::string& key, const Json::Value& value, bool isArrayElement, size_t index);

		/// <summary>
		/// _parseHelperList - Vector containing the addresses of all helpers associated with this parse coordinator.
		/// </summary>
		Vector<IJsonParseHelper*> _parseHelperList;

		/// <summary>
		/// _sharedData - address to the shared data associated with this parse coordinator.
		/// </summary>
		SharedData* _sharedData;

		/// <summary>
		/// _isClone - member that indicates whether or not this coordinator is a clone of another - an indicator of whether or not this coordinator needs to
		/// be responsible for the memory of it's sharedData and helpers.
		/// </summary>
		bool _isClone = false;

		/// <summary>
		/// _fileName - member that stores the name/path of the last file to be parsed by this coordinator.
		/// </summary>
		std::string _fileName;
	};

}