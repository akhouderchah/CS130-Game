# tetrad-game
A simple game built on top of a custom-built Entity System. The ES itself is designed to work for any game of small to medium scale.
***

#### Progress
###### Current Progress
- [x] Build Entity System (Entities, Components, and their manangers)
    - [x] Implement Component and Entity Managers
- [ ] Build Game System
    - [x] Build basic interface with GLFW
    - [ ] Create the needed Components and Systems for audio, graphics, input, etc
	    - [x] Build the Draw System using OpenGL for rendering
		- [ ] Build the Audio System using OpenAL
	- [ ] Build the Lisp Interpreter for configuration and scripting
    - [x] Create a logging system
- [ ] Build the actual game
    - [ ] Create the needed Components and Systems for game-specific functionality
    - [x] Create the visual assets (mainly textures)
    - [ ] Create the audio assets
- [ ] Refine
    - [ ] Visual tweaking
    - [ ] Performance tune-up

> Note: This is a rough estimate of what needs to be done. The actual development process will not be nearly so rigidly divided between the different elements of the project. As the project progresses and profiling brings more insight, the Entity and Game systems may have to be modified to improve performance/flexibility.

###### Changes to be made
The file GENERAL.todo contains the most important changes (or "todo"s) to be made, ranking the needed changes from Priority 1 (absolutely essential) to Priority 5 (would be nice to implement). While GENERAL.todo will not contain all the work to be done in the codebase, it will contain those that demand the most attention.
***

#### Notes of interest
- Due to the way that static initialization works in C++, the log system can't be reliably used for any set-up that happens before the execution of main() (take, for example, the adding of ComponentManagers to EntityManager::s_pComponentManagers). Try to avoid static initialization, opting instead to use an Initialize() method to set-up classes and such.
- Entity IDs start at 1, where ID 0 is reserved for the "null entity" at static initialization. Along with the null entity, every component type has a null component, also created at static initialization time (and passed the entity ID 0). Thus systems seeking to iterate over all their components must start with the component ID 1, instead of 0. This also means that if a component's constructor executes code that does not work until after static initialization (e.g. OpenGL code), that code should be enclosed in an if statement ensuring that m_Entity != nullEntity.

#### Directory Structure
Source code is structured such that related functionality is kept together. Public header files (that is, header files that will be directly included by source code outside of the current directory) reside at the top-level of the directories below. Implementation files reside inside the "_private" subdirectory located in the directory containing the corresponding header file. Private header files (ones that contain functionality specific only to the source code within the current directory) are kept inside the "_inc" subdirectories. The directory structure is shown below.

* /src - contains all the source code used in the project
    * /src/core - code that the game system relies on, but is not necessarily related to the system itself (data structures, functions/typedefs for platform independence, etc)
    * /src/engine - code for the game system (Entity/Component managers, memory/resource managers, etc)
    * /src/game - code that builds on top of the base engine (most importantly, the systems and components for the engine)
    * /src/external - code from APIs, libraries, etc
* /assets - parent folder for specific asset sub-folders
    * /assets/audio - folder for sound files
	* /assets/fonts - folder for font files
	* /assets/shaders - folder for glsl shader files
    * /assets/textures - folder for texture files

***

Copyright (c) 2016 Alex Khouderchah
