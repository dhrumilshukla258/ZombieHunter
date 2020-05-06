# ZombieHunter
A 2D game to go around and kill some zombies.
Made on a Custom Game Engine I worked on during the 1st Sem of my Masters Program. 

# GameEngine1.0
Game Engine developed during the 1st Semester of my Master Program. Its an individual project.

This is my first attempt of developing the game engine from scratch. It's a 2D driven Component Based Engine developed using C++/OpenGl. I developed 2D side platformer game using this engine, both engine and game were developed over a course of 2 months.

Some of the features of the engine: Object Factory: Creates components for the game objects. Serializing the game objects from json files. Creates objects from a paradigm.

Input Manager: An input manager for managin input events. KeyBoard and Mouse Events listener and dispatcher system.

2D collision System. Impulse based 2D Collision, It supports Reflection and timed based collision also.

Graphics: OpenGL based hardware accelerated graphics. A multiple Sprite Animation and sprite sheet support for a single game object.

Message Handling and Event System. Subscription based Event Handling. Supports time based events as well. Supports Message Delegation.

Memory Management: Creating Pools of game objects and components. Implemented UnorderedMaps for quick access of the data.

Miscellaneous: A json parser for data driven development.
