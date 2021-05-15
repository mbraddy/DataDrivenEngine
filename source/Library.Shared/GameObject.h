#pragma once
#include "Attributed.h"
#include "GameTime.h"
#include "GameClock.h"
#include "IFactory.h"
#include "Action.h"

namespace FieaGameEngine
{
	class GameObject;
	class Action;
	class EventQueue;

	/// <summary>
	/// GameState class - wrapper containing a pointer to the current game time.
	/// </summary>
	class GameState
	{
	public:
		/// <summary>
		/// GetGameTime - Gets the current game time.
		/// </summary>
		/// <returns>Reference to a GameTime</returns>
		GameTime& GetGameTime();

		/// <summary>
		/// SetGameTime - Sets the currentGameTime. Used to update the timer.
		/// </summary>
		/// <param name="gameTime">The updated Game time to set currentGameTime to.</param>
		void SetGameTime(GameTime& gameTime);

		/// <summary>
		/// GetCurrentAction - Returns pointer to the current action
		/// </summary>
		/// <returns>Action Pointer to the current action being processed.</returns>
		Action* GetCurrentAction();

		/// <summary>
		/// SetCurrentAction - Sets the currentAction pointer to the action passed in.
		/// </summary>
		/// <param name="action">Current action to be passed in to the game state.</param>
		void SetCurrentAction(Action& action);

		/// <summary>
		/// GetCurrentGameObject - Returns pointer to the current game object
		/// </summary>
		/// <returns>Game Object pointer to the current game object being processed.</returns>
		GameObject* GetCurrentGameObject();

		/// <summary>
		/// SetCurrentGameObject - Sets the current Game Object pointer to the Game Object passed in.
		/// </summary>
		/// <param name="object">Current Game Object to be passed into the game state.</param>
		void SetCurrentGameObject(GameObject& object);

		/// <summary>
		/// GetEventQueue - Gets a pointer to the event queue
		/// </summary>
		/// <returns>Pointer to the event queue.</returns>
		EventQueue* GetEventQueue();

		/// <summary>
		/// SetEventQueue - Sets the event queue pointer to the passed in event queue.
		/// </summary>
		/// <param name="queue">Reference to the event queue.</param>
		void SetEventQueue(EventQueue& queue);

	private:
		/// <summary>
		/// _currentGameTime - Pointer to a GameTime reference containing the current game time.
		/// </summary>
		GameTime* _currentGameTime = nullptr;

		/// <summary>
		/// _currentAction - Pointer to the current action being processed.
		/// </summary>
		Action* _currentAction = nullptr;

		/// <summary>
		/// _currentGameObject - Pointer to the current game object being processed.
		/// </summary>
		GameObject* _currentGameObject = nullptr;

		/// <summary>
		/// _eventQueue - Pointer to the event queue
		/// </summary>
		EventQueue* _eventQueue = nullptr;
	};

	/// <summary>
	/// GameObject - Attributed Scope that forms the basis for the hierarchy of the entire Game. Everything in the game will be a GameObject or a child of it.
	/// </summary>
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed)

	public:
		/// <summary>
		/// Constructor - GameObject default constructor that calls Attributed with GameObject::TypeIdClass();
		/// </summary>
		GameObject();

		/// <summary>
		/// Copy Constructor - GameObject copy constructor - Defaulted.
		/// </summary>
		/// <param name="other">The GameObject to be copied.</param>
		GameObject(const GameObject& other) = default;

		/// <summary>
		/// Move Constructor - GameObject move constructor - Defaulted.
		/// </summary>
		/// <param name="other">The GameObject to be moved.</param>
		GameObject(GameObject&& other) noexcept = default;

		/// <summary>
		/// Copy assignment operator -  Defaulted.
		/// </summary>
		/// <param name="other">The GameObject to be copied.</param>
		GameObject& operator=(const GameObject& other) = default;

		/// <summary>
		/// Move assignment operator - Defaulted.
		/// </summary>
		/// <param name="other">The GameObject to be moved.</param>
		GameObject& operator=(GameObject&& other) noexcept = default;

		/// <summary>
		/// Virtual destructor - Defaulted.
		/// </summary>
		/// <param name="other">The GameObject to be copied.</param>
		virtual ~GameObject() = default;

		/// <summary>
		/// Name - Returns the _name member of the GameObject.
		/// </summary>
		/// <returns>std::string _name member</returns>
		const std::string& Name() const;

		/// <summary>
		/// SetName - Sets the name to the L-value string passed in.
		/// </summary>
		/// <param name="newName">L-Value reference to the desired new name.</param>
		void SetName(const std::string& newName);

		/// <summary>
		/// SetName - Sets the name to the R-value string passed in.
		/// </summary>
		/// <param name="newName">R-Value reference to the desired new name.</param>
		void SetName(std::string&& newName);

		/// <summary>
		/// Transform - Returns the _transform matrix of the GameObject
		/// </summary>
		/// <returns>mat4 - the transform matrix of the object.</returns>
		const mat4& Transform() const;

		/// <summary>
		/// SetTransform - Sets the transform matrix to the L-value passed in.
		/// </summary>
		/// <param name="matrix">L-value reference to the desired transform matrix</param>
		void SetTransform(const mat4& matrix);

		/// <summary>
		/// SetTransform - Sets the transform matrix to the R-value passed in.
		/// </summary>
		/// <param name="matrix">R-value reference to the desired transform matrix</param>
		void SetTransform(mat4&& matrix);

		/// <summary>
		/// Children - Returns the datum associated with any children game objects this GameObject contains.
		/// </summary>
		/// <returns>reference to the datum containing the game objects stored as children.</returns>
		Datum& Children();

		/// <summary>
		/// Actions - Returns the datum associated with any Actions this GameObject contains.
		/// </summary>
		/// <returns>reference to the datum containing the Actions stored as children.</returns>
		Datum& Actions();

		/// <summary>
		/// CreateGameObject - Creates an instance of a game object from a Factory that has been registered in the Factory manager.
		/// </summary>
		/// <param name="className">String containing the name of the Class that you wish to create.</param>
		/// <param name="instanceName">Key that you wish for the instantiated class to be associated with in the GameObject hierarchy.</param>
		/// <returns>Pointer to the game object that is created.</returns>
		/// <exception cref="std::runtime_error">Attempting to instantiate a class that has not been registered in the IFactory of Scopes will
		/// cause a runtime_error.</exception>
		GameObject* CreateGameObject(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// CreateAction - Convenience method that Creates a factory scope based on the passed in class name and adopts it to the gameObject paired
		/// with the associated instanceName.
		/// </summary>
		/// <param name="className">Name of the Factory class you wish to create.</param>
		/// <param name="instanceName">Key string that you wish to associate with the action.</param>
		///	<exception cref="std::runtime_error">Passing in a className that has no registered factory will cause a runtime error.</exception>
		void CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Update - Calls all the update functions of all children in the hierarchy.
		/// </summary>
		/// <param name="gameState">The current gamestate (passed to all children).</param>
		virtual void Update(GameState& gameState);

		/// <summary>
		/// Clone - Clone method that creates a newly instantiated copy of this GameObject.
		/// </summary>
		/// <returns>Pointer to the newly instantiated copy.</returns>
		virtual gsl::owner<GameObject*> Clone() const override;

		/// <summary>
		/// Signatures - Returns the vector of signatures describing the prescribed members of the GameObject. Used for TypeManager Registration.
		/// </summary>
		/// <returns>Vector of signatures of all prescribed objects.</returns>
		static const Vector<Signature> Signatures();
	protected:
		/// <summary>
		/// TypeID constructor - Constructor provided for children to call in their default constructors. Registers the Type ID of "this" game object as
		/// whatever is passed in as a parameter. 
		/// </summary>
		/// <param name="typeID">The Type ID of the calling child class.</param>
		explicit GameObject(RTTI::IdType typeID);

		/// <summary>
		/// _name - private member containing the name identifier of the GameObject.
		/// </summary>
		std::string _name;

		/// <summary>
		/// _transform - private member containing the transform matrix of the GameObject.
		/// </summary>
		mat4x4 _transform = {0, 0, 0, 0,
							 0, 0, 0, 0,
							 0, 0, 0, 0,
							 0, 0, 0, 0};

		static const size_t _actionsIndex = 3;

		static const size_t _childrenIndex = 4;

		HashMap<std::string, Datum*> _updateTable;

		HashMap<std::string, Datum*> _actionTable;
	};
}

