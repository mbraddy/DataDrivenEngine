#include "pch.h"
#include "GameObject.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject)

#pragma region GameState Methods

	GameTime& GameState::GetGameTime()
	{
		return *_currentGameTime;
	}

	void GameState::SetGameTime(GameTime& gameTime)
	{
		_currentGameTime = &gameTime;
	}

	Action* GameState::GetCurrentAction()
	{
		return _currentAction;
	}

	void GameState::SetCurrentAction(Action& action)
	{
		_currentAction = &action;
	}

	GameObject* GameState::GetCurrentGameObject()
	{
		return _currentGameObject;
	}

	void GameState::SetCurrentGameObject(GameObject& object)
	{
		_currentGameObject = &object;
	}

#pragma endregion

	const Vector<Signature> GameObject::Signatures()
	{
		FieaGameEngine::Vector<Signature> signatureVector =
		{
			{"Name", Datum::DatumType::String, 1, offsetof(GameObject, _name)},
			{"Transform", Datum::DatumType::Matrix, 1, offsetof(GameObject, _transform)},
			{"Actions", Datum::DatumType::Table, 0, 0},
			{"Children", Datum::DatumType::Table, 0, 0}
		};

		return signatureVector;
	}

	GameObject::GameObject() :
		Attributed(TypeIdClass())
	{
		assert(_orderList.At(_actionsIndex)->first == "Actions");
		assert(_orderList.At(_actionsIndex)->second.Type() == Datum::DatumType::Table);
		assert(_orderList.At(_childrenIndex)->first == "Children");
		assert(_orderList.At(_childrenIndex)->second.Type() == Datum::DatumType::Table);

		_updateTable.Insert(std::make_pair("Children", &(_orderList[_childrenIndex]->second)));
		_actionTable.Insert(std::make_pair("Actions", &(_orderList[_actionsIndex]->second)));
	}

	GameObject::GameObject(RTTI::IdType typeID) :
		Attributed(typeID)
	{
		assert(_orderList.At(_actionsIndex)->first == "Actions");
		assert(_orderList.At(_actionsIndex)->second.Type() == Datum::DatumType::Table);

		assert(_orderList.At(_childrenIndex)->first == "Children");
		assert(_orderList.At(_childrenIndex)->second.Type() == Datum::DatumType::Table);

		_updateTable.Insert(std::make_pair("Children", &(_orderList[_childrenIndex]->second)));
		_actionTable.Insert(std::make_pair("Actions", &(_orderList[_actionsIndex]->second)));
	}

	Datum& GameObject::Actions()
	{
		return _orderList[_actionsIndex]->second;
	}

	void GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		const IFactory<Scope>* fp;
		Scope* newAction = fp->Create(className);

		if (newAction == nullptr)
		{
			throw std::runtime_error("Invalid Class name - not a registered factory.");
		}

		assert(newAction->Is(Action::TypeIdClass()));

		//	Adopts the action into the Actions prescribed member.
		Adopt(*newAction, instanceName);

		//	Gets reference to the newly adopted Scope's datum
		Datum& instanceDatum = Append(instanceName);

		//	Inserts into the Update Map for quick lookup
		_actionTable.Insert(std::make_pair(instanceName, &instanceDatum));

	}

	void GameObject::Update(GameState& gameState)
	{
		//	Update the game state to reference this game Object
		gameState.SetCurrentGameObject(*this);

		for (auto it = _actionTable.begin(); it != _actionTable.end(); ++it)
		{
			auto& actionsDatum = *(it->second);
			for (size_t i = 0; i < actionsDatum.Size(); ++i)
			{
				assert(actionsDatum[i].Is(Action::TypeIdClass()));
				static_cast<Action&>(actionsDatum[i]).Update(gameState);
			}
		}


		for (auto it = _updateTable.begin(); it != _updateTable.end(); ++it)
		{
			auto& childrenDatum = *(it->second);
			for (size_t i = 0; i < childrenDatum.Size(); ++i)
			{
				assert(childrenDatum[i].Is(GameObject::TypeIdClass()));
				static_cast<GameObject&>(childrenDatum[i]).Update(gameState);
			}
		}
	}

	typename GameObject* GameObject::CreateGameObject(const std::string& className, const std::string& instanceName)
	{
		const IFactory<Scope>* fp;
		Scope* newObject = fp->Create(className);

		if (newObject == nullptr)
		{
			throw std::runtime_error("Invalid Class name - not a registered factory.");
		}

		assert(newObject->Is(GameObject::TypeIdClass()));
		
		//	Adopts GameObject Into this scope
		Adopt(*newObject, instanceName);

		//	Gets reference to the newly adopted Scope's datum
		Datum& instanceDatum = Append(instanceName);

		//	Inserts into the Update Map for quick lookup
		_updateTable.Insert(std::make_pair(instanceName, &instanceDatum));

		return static_cast<GameObject*>(newObject);
	}

	const std::string& GameObject::Name() const
	{
		return _name;
	}

	void GameObject::SetName(const std::string& newName)
	{
		_name = newName;
	}

	void GameObject::SetName(std::string&& newName)
	{
		_name = std::move(newName);
	}

	const mat4& GameObject::Transform() const
	{
		return _transform;
	}

	void GameObject::SetTransform(const mat4& matrix)
	{
		_transform = matrix;
	}

	void GameObject::SetTransform(mat4&& matrix)
	{
		_transform = std::move(matrix);
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	Datum& GameObject::Children()
	{
		return _orderList[_childrenIndex]->second;
	}

	EventQueue* GameState::GetEventQueue()
	{
		return _eventQueue;
	}

	void GameState::SetEventQueue(EventQueue& queue)
	{
		_eventQueue = &queue;
	}
}

