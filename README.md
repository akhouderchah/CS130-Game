# Tetrad Engine
A 3D engine based on the Entity System architecture. The ES itself is designed to work for any game of small to medium scale.
***

#### Building
1. Ensure your system has CMake installed. This will be used to generate a particular build system (e.g. Makefile or Visual Studio project) from CMakeLists.txt.
1. Run CMake, specify the location of the "src" folder, and indicate that the build should be placed inside a subfolder of the "bin" directory (e.g. ~/CS130-Game/bin/debug)
1. Run "configure", uncheck all of the GLFW_* options, and then run "generate".
1. For Visual Studio, after opening the .sln file, one last step is to right-click on the "tetrad-game" project and select "Set as StartUp Project".

#### Building Documentation
1. Ensure that your system has doxygen installed, and run CMake (if you have an existing CMake build, simply follow the same process as described in the "Recompiling with new files" section; no need to use the CMake gui directly after the first time)
1. With makefiles, use the command "make doc" to build the documentation. With Visual Studio, build the project 'doc'
1. If you wish to view the documentation website, go into the 'html' folder in your build folder and open index.html. The main page is empty, but the Class Hierarchy page may be quite useful
1. On Linux, you can also go into the 'latex' folder, type 'make', and view the resulting pdf document

#### General Description
This is a component-entity system, meaning that game objects are created by assigning components to game entities (which are simply integers--they contain no game functionality themselves). Systems then act on components in order to provide the desired functionality. That is, components do not implement functionality. Instead, components store the data (and provide the interface necessary to modify such data) that systems will need to implement functionality. For example, a DrawComponent would store (or reference) information on the 3D model to draw and the texture to apply to it, but the DrawSystem performs the actual rendering.

###### Convention Notes
Please conform to the coding style used in the existing project.
Main points are:
* Vertically-aligned braces (no K&R style)
* Non-static member variable names begin with 'm_'
* Static member variable names begin with 's_'
* Document classes and important methods/functions in the Doxygen style (see src/engine/EntityManager.h, for example)
* Pointer variable names begin with 'p' (member variables that are pointers start with 'm_p' or 's_p')
* Don't write directly to cout/cerr. Use DEBUG_LOG("Message\n"), LOG("Message\n"), ERROR("Error message\n", EEB_CONTINUE), or ERROR("Error message\n", EEB_EXIT), depending on the context

###### Creating a new Component
1. Make sure the Component is designed so that the relevant Systems are actually performing the desired functionality, and the Component itself is simply providing the data necessary for the System to do so. If this isn't the case, redesign.
1. Create a new header in src/game/component/, and its corresponding cpp in src/game/component/_private
1. In the header file, #include "Base.h" and "IComponent.h"
1. Create the component class as according to these rules:
    * It publicly inherits from IComponent
    * There is a constructor taking a single Entity as its argument, and it passes this entity to the IComponent constructor in the member initialization list
	* It declares and defines the method "void Refresh()" (see the next bullet point for more information on this; also you'll get a compile error if you forget to do this)
	* If this component relies on another component in an entity, create a private pointer to that component in the class (let's just call it m_pComp for the rest of this bullet point). Then in the Refresh() method, write m_pComp = EntityManager::GetComponent<COMPONENT_TYPE>(m_Entity);, where COMPONENT_TYPE is the type of the component
1. In the relevant Systems that will use this, you must create a member variable of type ConstVector<COMPONENT_TYPE*>, and in the member initialization list, set that variable to EntityManager::GetAll<COMPONENT_TYPE>()
1. Document the component class and any important methods in the Doxygen style (see the Convention Notes for more information).
1. See the note below regarding compiling with new files, then recompile!

###### Creating a new System
1. Create a new header in src/game/system/, and a corresponding cpp in src/game/system/_private
1. In the header file, ensure to #include "ISystem.h"
1. Create the System class as according to these rules:
    * It publicly inherits from ISystem
	* It declares and defines methods: bool Initialize(), void Shutdown(), void Tick(deltaTime_t dt)
	* If/when iterating over all components of a certain type (as is likely in the Tick method), ensure to start at 1 rather than 0. The 0th element in the array of components is the null array, which should not be used.
1. Document the system class and any important methods in the Doxygen style (see the Convention Notes for more information).
1. In dynamically allocate a system instance and push_back to m_pSystems in TetradGame::AddSystems

###### Recompiling with new files
When new files are added to the project, CMake must be retriggered in order to build with the new files. The easiest way to do this is simply to touch CMakeLists.txt. "touch" on Windows can be done with "copy /b filename.ext +,,", but it's also possible simply to download the Windows version of the standard Linux tools instead. You could also rebuild everything in Visual Studio without touching CMakeLists.txt for the same effect, but make sure that all projects are being rebuilt, not just the StartUp project.

###### Finding work items
See the Issues on the Github page to find unassigned work items.
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

