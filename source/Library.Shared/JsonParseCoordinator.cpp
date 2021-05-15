#include "pch.h"
#include "JsonParseCoordinator.h"
#include "json/json.h"
#include <istream>
#include <fstream>
#include <cassert>
#include <limits>

namespace FieaGameEngine
{
#pragma region SharedData Methods

	RTTI_DEFINITIONS(SharedData)

	JsonParseCoordinator* const SharedData::GetJsonParseCoordinator() const
	{
		return _parseMaster;
	}

	size_t SharedData::Depth() const
	{
		return _depth;
	}

	void SharedData::DecrementDepth()
	{
		assert(_depth > 0);
		--_depth;
	}

	void SharedData::IncrementDepth()
	{
		assert(_depth < std::numeric_limits<size_t>::max());
		++_depth;
	}

	void SharedData::SetJsonParseCoordinator(JsonParseCoordinator* parseMasterAddress)
	{
		assert(parseMasterAddress != nullptr);
		_parseMaster = parseMasterAddress;
	}

	void SharedData::Initialize()
	{
		_depth = 0;
	}

	void SharedData::CleanUp()
	{
	}

#pragma endregion

#pragma region ParseCoordinator Methods

	JsonParseCoordinator::JsonParseCoordinator(SharedData& data) :
		_sharedData(&data)
	{
		_sharedData->SetJsonParseCoordinator(this);
	}

	JsonParseCoordinator::~JsonParseCoordinator()
	{
		if (_isClone)
		{
			Clear();
		}
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		_sharedData(other._sharedData), _fileName(std::move(other._fileName)), _isClone(other._isClone), _parseHelperList(std::move(other._parseHelperList))
	{	
		_sharedData->SetJsonParseCoordinator(this);

		other._sharedData = nullptr;
		other._isClone = false;
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		if (this != &other)
		{
			//	Need to delete the parse helpers in the current vector
			if (_isClone)
			{
				Clear();
			}

			_sharedData = other._sharedData;
			_sharedData->SetJsonParseCoordinator(this);

			_parseHelperList = std::move(other._parseHelperList);
			_fileName = std::move(other._fileName);
			_isClone = other._isClone;

			other._isClone = false;
			other._sharedData = nullptr;
		}

		return *this;
	}

	void JsonParseCoordinator::Initialize()
	{
		for (auto helper : _parseHelperList)
		{
			helper->Initialize();
		}
		_sharedData->Initialize();
	}

	void JsonParseCoordinator::CleanUp()
	{
		for (auto helper : _parseHelperList)
		{
			helper->CleanUp();
		}
		_sharedData->CleanUp();
	}

	void JsonParseCoordinator::Clear()
	{
		for (auto* helper : _parseHelperList)
		{
			delete helper;
		}

		delete _sharedData;
	}

	gsl::owner<JsonParseCoordinator*> JsonParseCoordinator::Clone()
	{
		SharedData* clonedData = _sharedData->Create();
		JsonParseCoordinator* clonedParser = new JsonParseCoordinator(*clonedData);
		clonedParser->_isClone = true;
		
		clonedParser->_parseHelperList.Reserve(_parseHelperList.Size());
		for (size_t i = 0; i < _parseHelperList.Size(); ++i)
		{
			clonedParser->_parseHelperList.PushBack(_parseHelperList[i]->Create());
		}

		clonedParser->_fileName = _fileName;

		return clonedParser;
	}

	void JsonParseCoordinator::AddHelper(const IJsonParseHelper& helper)
	{
		if (_isClone)
		{
			throw std::runtime_error("Cannot modify helpers of a cloned parse coordinator.");
		}

		//	Prevent Adding duplicate helpers of the same type
		for (auto helperInList : _parseHelperList)
		{
			if (helperInList->TypeIdInstance() == helper.TypeIdInstance())
			{
				throw std::runtime_error("Attempting to insert a helper of a type that already exists within the Parse Coordinator.");
			}
		}

		_parseHelperList.PushBack(const_cast<IJsonParseHelper*>(&helper));
	}

	void JsonParseCoordinator::RemoveHelper(const IJsonParseHelper& helper)
	{
		if (_isClone)
		{
			throw std::runtime_error("Cannot modify helpers of a cloned parse coordinator.");
		}

		_parseHelperList.Remove(const_cast<IJsonParseHelper*>(&helper));
	}

	size_t JsonParseCoordinator::GetHelperCount() const
	{
		return _parseHelperList.Size();
	}

	SharedData* JsonParseCoordinator::GetSharedData()
	{
		return _sharedData;
	}

	void JsonParseCoordinator::SetSharedData(SharedData& sharedData)
	{
		if (_isClone)
		{
			throw std::runtime_error("Cannot modify shared data of a cloned parse coordinator.");
		}

		_sharedData = &sharedData;
	}

	const std::string& JsonParseCoordinator::GetFileName()
	{
		return _fileName;
	}

	void JsonParseCoordinator::Parse(const std::string& key, const Json::Value& value, bool isArrayElement, size_t index)
	{
		if (value.isObject())
		{
			_sharedData->IncrementDepth();

			for (auto* helper : _parseHelperList)
			{
				if (helper->StartHandler(*(_sharedData), key, value, isArrayElement, index))
				{
					ParseMembers(value);
					helper->EndHandler(*(_sharedData), key);
					break;
				}
			}

			_sharedData->DecrementDepth();
		}
		else if (value.isArray())
		{
			size_t i = 0;
			for (const auto& element : value)
			{
				if (element.isObject())
				{
					_sharedData->IncrementDepth();
					ParseMembers(element, true, i);
					_sharedData->DecrementDepth();
				}
				else
				{
					Parse(key, element, true, i);
				}
				++i;
			}
		}
		else
		{
			for (auto* helper : _parseHelperList)
			{
				if (helper->StartHandler(*(_sharedData), key, value, isArrayElement, index))
				{
					helper->EndHandler(*(_sharedData), key);
				}
			}
		}
	}

	void JsonParseCoordinator::ParseMembers(const Json::Value& root, bool isArrayElement, size_t index)
	{
		if (root.size() > 0)
		{
			const auto keys = root.getMemberNames();
			for (const auto& key : keys)
			{
				Parse(key, root[key], isArrayElement, index);
			}
		}
	}

	void JsonParseCoordinator::Parse(std::string& jsonString)
	{
		std::stringstream stream;
		stream << jsonString;
		Parse(stream);
	}

	void JsonParseCoordinator::Parse(std::istream& jsonStream)
	{
		if (_sharedData == nullptr || _parseHelperList.Size() == 0_z)
		{
			return;
		}

		Initialize();

		Json::Value root;
		jsonStream >> root;

		_sharedData->IncrementDepth();
		ParseMembers(root);
		_sharedData->DecrementDepth();

		CleanUp();
	}

	void JsonParseCoordinator::ParseFromFile(std::string& fileName)
	{
		std::ifstream ifs;
		ifs.open(fileName);

		if (ifs.is_open())
		{
			Parse(ifs);
			_fileName = std::move(fileName);
			ifs.close();
		}
	}

#pragma endregion
}
