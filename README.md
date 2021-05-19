"# DataDrivenEngine" 

This C++ Data-Driven Game Engine was developed by me in pursuit of my graduate degree at the Florida Interactive Entertainment Academy. The project was broken down into several segments and all relevant code can be viewed via the GitHub link at the top of the page. Upon completion we were to make a very simple C++ game utilizing the engine. At the bottom of this page will be a demo of the game made in this engine.

Cross Platform Solution - The goal was to educate us on how to build a project for multiple platform targets.

Templated Singly Linked List - Build our own singly linked list container while introducing templates and Iterators

Templated Vector - Build our own templated vector container which would be used in engine development in place of the standard library’s

Templated HashMap - Build our own templated hashmap which would also be used heavily within the engine.

Datum - A massive data storage class which allows us to utilize RTTI and determine a data container type at runtime. 

Scope - A Table class that is used in conjunction with Datum to form a hierarchal database. Scopes are tables of Datums, and datums can be of type scope.

Attributed - Represents an attributed scope - that is a scope to which we can pre-define attributes in it that can get populated at run-time. This allows us to mirror native compile time classes with scopes and datums that we pre-define attributes for within a type manager.

Parse Coordinator and Table Parser - A system that establishes the chain of responsibility pattern. Json files are passed to the parse coordinator which looks at any added/existing parse helpers that have been created and added to it’s list. If there is a parse helper that is capable of parsing the Json file’s grammar, then it takes the responsibility for doing so. This parses out the information in the Json file and creates Scopes and Datums populated with the provided information.

Factory - Created an abstract base class and associated macro that allows us to create concrete classes based on the name received from information passed in through Json. We emulate a virtual constructor in C++ and store the factories in a Factory Manager so that we can instantiate objects of a given type at runtime by name.

GameObject - A class which is an Attributed Scope that contains prescribed members that allow the establishment of a hierarchy of game objects within a scene. Game Objects can be anything and be parent to any other game object.

Action - A class which is another Attributed Scope that represents components/actions for Game Objects. These can be attached to GameObjects and provide functionality to those objects either by directly modifying them or by calling other actions / events.

Event and Reaction - A system that establishes the observer pattern within the game engine. Anything that inherits from IEventSubscriber can then opt to subscribe to specific EventPublishers. This allows for actions to be executed only when the Publisher delivers the event to the subscriber and allows us to skip polling every single action on Update ticks.
