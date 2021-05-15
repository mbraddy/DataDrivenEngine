#include "pch.h"
#include "JsonTableParseHelper.h"
#include "IFactory.h"
namespace FieaGameEngine
{
    RTTI_DEFINITIONS(SharedTableData)

    RTTI_DEFINITIONS(JsonTableParseHelper)

    gsl::owner<SharedTableData*> SharedTableData::Create()
    {
        return new SharedTableData();
    }

    SharedTableData::SharedTableData(Scope& s)
    {
        _scopePointer = &s;
    }

    void SharedTableData::SetRootScope(Scope& s)
    {
        _scopePointer = &s;
    }

    Scope* SharedTableData::GetRootScope()
    {
        return _scopePointer;
    }

    gsl::owner<JsonTableParseHelper*> JsonTableParseHelper::Create()
    {
        return new JsonTableParseHelper();
    }

    void JsonTableParseHelper::SetDatumValue(Datum& datum, const Json::Value& value, size_t index)
    {
        if (datum.OwnsData())
        {
            datum.PushBackFromJsonValue(value);
        }
        else
        {
            datum.SetFromJsonValue(value, index);
        }
    }

    void JsonTableParseHelper::Initialize()
    {
        IJsonParseHelper::Initialize();
    }

    void JsonTableParseHelper::CleanUp()
    {
        IJsonParseHelper::CleanUp();
    }

    bool JsonTableParseHelper::StartHandler(SharedData& data, const std::string& key, const Json::Value& object, bool isArray, size_t index)
    {
        if (!data.Is(SharedTableData::TypeIdClass()))
        {
            return false;
        }

        SharedTableData& tableData = reinterpret_cast<SharedTableData&>(data);
     
        if (key == "value"s)
        {
            assert(_contextStack.IsEmpty() == false);
            StackFrame& currentContext = _contextStack.Peek();
            if (currentContext._datum.Type() == Datum::DatumType::Table)
            {
                assert(currentContext._className.empty() == false);
                Scope* factoryScope = IFactory<Scope>::Create(currentContext._className);
                if (factoryScope == nullptr)
                {
                    throw std::runtime_error("Attempted to create an instance of a factory that is not registered in the factory table.");
                }

                assert(currentContext._attributeName != nullptr);
                currentContext._context->Adopt(*factoryScope, *currentContext._attributeName);
                _contextStack.Push(StackFrame{ &key, factoryScope, currentContext._className, currentContext._datum });
            }
            else
            {
                SetDatumValue(currentContext._datum, object, index);
            }
        }
        else if (key == "type"s)
        {
            assert(_contextStack.IsEmpty() == false);
            const StackFrame& currentContext = _contextStack.Peek();
            currentContext._datum.SetType(Datum::_setTypeJsonTableParseMap.At(object.asString()));
        }
        else if (key == "class"s)
        {
            assert(_contextStack.IsEmpty() == false);
            StackFrame& currentContext = _contextStack.Peek();
            currentContext._className = object.asString();
        }
        else
        {
            Scope* context = _contextStack.IsEmpty() ? tableData.GetRootScope() : _contextStack.Peek()._context;
            Datum& datum = context->Append(key);
            _contextStack.Push(StackFrame{ &key, context, "Scope"s, datum });
        }

        UNREFERENCED_LOCAL(isArray);

        return true;
    }

    bool JsonTableParseHelper::EndHandler(SharedData& data, const std::string& key)
    {
        if (!data.Is(SharedTableData::TypeIdClass()))
        {
            return false;
        }

        if (_contextStack.IsEmpty() == false)
        {
            const StackFrame& currentContext = _contextStack.Peek();
            if (currentContext._attributeName == &key)
            {
                _contextStack.Pop();
            }
        }
        return true;
    }

}