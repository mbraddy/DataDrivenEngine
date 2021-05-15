#pragma once
#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include "Scope.h"
#include <tuple>

namespace FieaGameEngine
{
    class JsonTableParseHelper;

    /// <summary>
    /// SharedTableData - Class that serves as a wrapper for a scope which is used to contain the parsed data from Json files (parsed by ParseCoordinator.cpp)
    /// </summary>
    class SharedTableData : public SharedData
    {
        RTTI_DECLARATIONS(SharedTableData, SharedData)

        friend JsonTableParseHelper;

    public:

        /// <summary>
        /// Constructor - Takes a reference to a scope and sets it's address as the root scope pointer for parsing the Json data.
        /// </summary>
        /// <param name="s">Reference to a scope that you want to hold the parsed data.</param>
        explicit SharedTableData(Scope& s);

        /// <summary>
        /// Copy Constructor - Deleted. This Shared data reference should only be able to be copied by the parse master during cloning.
        /// </summary>
        /// <param name="other">Data to be copied.</param>
        SharedTableData(const SharedTableData& other) = delete;

        /// <summary>
        /// Move Constructor - Deleted. This Shared data reference shouldn't be moved. It's just a wrapper for the scope Move the scope instead.
        /// </summary>
        /// <param name="other">Data to be moved.</param>
        SharedTableData(SharedTableData&& other) = delete;

        /// <summary>
        /// Copy Assignment Operator - Deleted. This Shared data reference should only be able to be copied by the parse master during cloning.
        /// </summary>
        /// <param name="other">Data to be copied.</param>
        SharedTableData& operator=(const SharedTableData& other) = delete;

        /// <summary>
        /// Move Assignment Operator - Deleted. This Shared data reference shouldn't be moved. It's just a wrapper for the scope Move the scope instead.
        /// </summary>
        /// <param name="other">Data to be moved.</param>
        SharedTableData& operator=(SharedTableData&& other) = delete;

        /// <summary>
        /// SetRootScope - Changes the scope pointer to a different scope.
        /// </summary>
        /// <param name="s">Scope you wish to set as the root scope for the parsed data.</param>
        void SetRootScope(Scope& s);

        /// <summary>
        /// GetRootScope - Gets the address of the root scope associated with this SharedData.
        /// </summary>
        /// <returns>Address of the root scope associated with this shared data.</returns>
        Scope* GetRootScope();

        /// <summary>
        /// Virtual Destructor - Required for the V-Table
        /// </summary>
        virtual ~SharedTableData() = default;

        /// <summary>
        /// Create - Emulates a Virtual Constructor.
        /// </summary>
        /// <returns>Reference to a newly created SharedTableData</returns>
        virtual gsl::owner<SharedTableData*> Create() override;

    private:

        /// <summary>
        /// Default Constructor, privated because you shouldn't be able to instantiate a SharedTableData without a scope reference, but Create() needs to be able
        /// to use it.
        /// </summary>
        SharedTableData() = default;

        /// <summary>
        /// _scopePointer - Pointer to the Scope that you wish to be the root of the parsed data.
        /// </summary>
        Scope* _scopePointer = nullptr;
    };

    /// <summary>
    /// JsonTableParseHelper - ParseHelper containing the grammar necessary to parse a Json file into Scopes and Datums for use in the engine.
    /// </summary>
    class JsonTableParseHelper : public IJsonParseHelper
    {
        RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)

        /// <summary>
        /// StackFrame - Represents a struct that is used to establish the correct hierarchy of data.
        /// </summary>
        struct StackFrame final
        {
            /// <summary>
            /// _attributeName - Name of the attribute associated with this stack frame
            /// </summary>
            const std::string* _attributeName;

            /// <summary>
            /// _context - Pointer to the scope of the current context in the hierarchy.
            /// </summary>
            Scope* _context;

            /// <summary>
            /// _className - Name of the class to be instantiated at this context.
            /// </summary>
            std::string _className;

            /// <summary>
            /// _datum - Reference to the datum paired with the _attributeName at this context frame.
            /// </summary>
            Datum& _datum;
        };

    public:

        /// <summary>
        /// Initialize - Doesn't do anything in this instance.
        /// </summary>
        virtual void Initialize() override;

        /// <summary>
        /// Cleanup - Doesn't do anything in this instance.
        /// </summary>
        virtual void CleanUp() override;

        /// <summary>
        /// StartHandler - Takes in a reference to a SharedData, a key string, a json value, a boolean indicating if that json value is an array,
        /// and an index. If the shared data type is a SharedTableData, it will parse the json::value with the key and build Scopes + datums out of the
        /// json being parsed.
        /// </summary>
        /// <param name="data">SharedData Reference, should be a SharedTableData for a valid parse.</param>
        /// <param name="key">Keystring associated with a parsed object's name.</param>
        /// <param name="object">Json::Value containing the data associated with the keystring.</param>
        /// <param name="isArray">Boolean indicating if the object is an array. Not used in this helper.</param>
        /// <param name="index">Index used for array parsing. Not used in this helper.</param>
        /// <returns>True if the data was handled, elsewise false.</returns>
        virtual bool StartHandler(SharedData& data, const std::string& key, const Json::Value& object, bool isArray, size_t index) override;

        /// <summary>
        /// EndHandler - Takes in a reference to a SharedData and a key string. Pops the stack frame associated with the key (if there is one)
        /// and adjusts the stack frame's scope pointer if it was altered.
        /// </summary>
        /// <param name="data">Reference to shared data. Must be SharedTableData to do anything.</param>
        /// <param name="key">Keystring associated with a parsed objects name. Used to pop from stack.</param>
        /// <returns>True if the data was handled, false otherwise.</returns>
        virtual bool EndHandler(SharedData& data, const std::string& key) override;

        /// <summary>
        /// Create - Emulates a virtual constructor. Used within the ParseMaster Clone method to make a clone of the helper.
        /// </summary>
        /// <returns>A pointer to a newly instantiated copy of this helper.</returns>
        virtual gsl::owner<JsonTableParseHelper*> Create() override;

    private:

        /// <summary>
        /// SetDatumValue - checks if the datum owns the data it contains and calls either Pushback (if it does) or Set (if it doesnt)
        /// </summary>
        /// <param name="datum">Reference to the datum receving the value.</param>
        /// <param name="value">Json value that is to be attached to the datum.</param>
        /// <param name="index">Index to set the data at (if data isn't owned by the datum).</param>
        void SetDatumValue(Datum& datum, const Json::Value& value, size_t index);

        /// <summary>
        /// _stack - Necessary to maintain proper hierarchy in terms of correctly associating nested scopes and their data members.
        /// </summary>
        Stack<StackFrame> _contextStack;
    };

}