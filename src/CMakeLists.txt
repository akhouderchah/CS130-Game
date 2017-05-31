cmake_minimum_required(VERSION 2.6)
project(tetrad-game)

# Version Number
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "Please use a different build directory than the source directory")
endif()


# Define asset path for use in the executable
set(ASSET_PATH ${CMAKE_SOURCE_DIR}/../assets/)

# Add the system type for the config file
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(SYSTEM_TYPE 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(SYSTEM_TYPE 2)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(SYSTEM_TYPE 3)
else(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(SYSTEM_TYPE 0)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Determine Compiler
if(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  message("Compiler is Clang")
  add_definitions(-DCOMPILER_IS_CLANG)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  message("Compiler is GNU")
  add_definitions(-DCOMPILER_IS_GCC)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
  message("Compiler is GCC")
  add_definitions(-DCOMPILER_IS_INTEL)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  message("Compiler is MSVC")
  add_definitions(-DCOMPILER_IS_MSVC
	  -DTW_STATIC
	  -DTW_NO_LIB_PRAGMA
	  -DTW_NO_DIRECT3D
	  -DGLEW_STATIC
	  -D_CRT_SECURE_NO_WARNINGS)
else()
  message("Compiler is unknown!")
endif()

# Determine Endianness
include (TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)
if(${BIG_ENDIAN})
  add_definitions(-DIS_BIG_ENDIAN)
endif()

# 32 vs 64 bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ADDRESS_WIDTH 64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ADDRESS_WIDTH 32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME} ${ADDRESS_WIDTH}")

# Debug by default - only runs for single configurations
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

find_package(OpenGL REQUIRED)

# Find OpenAL - script works well for Linux + Mac, manually set for Windows
if(WIN32)
	set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/openal/include)
	set(OPENAL_LIBRARY ${CMAKE_SOURCE_DIR}/external/openal/libs/Win${ADDRESS_WIDTH}/OpenAL32.lib)
else()
	find_package(OpenAL	REQUIRED)
endif()

#find_package(wxWidgets COMPONENTS core base adv REQUIRED)
#include( "${wxWidgets_USE_FILE}" )
add_subdirectory(external)

# Define _DEBUG on debug builds
if (MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /DBUILD_TYPE=0 /MP")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DBUILD_TYPE=1 /MP")
elseif(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-Og -D_DEBUG -DBUILD_TYPE=0 -Wall -g -ggdb -pedantic-errors -Wextra -lprofiler -fsanitize=address -fsanitize=leak -fsanitize=undefined")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DBUILD_TYPE=1 -Wall -pedantic-errors -lprofiler -Wextra")
endif(MSVC)

configure_file(
	${PROJECT_SOURCE_DIR}/Config.h.in
	${PROJECT_BINARY_DIR}/Config.h )

# add Doxygen support
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating documentation with Doxygen" VERBATIM
		)
endif(DOXYGEN_FOUND)

set(CMAKE_CXX_STANDARD 11)

## Header and Source files
file(GLOB_RECURSE CORE_SRC "./core/*.cpp")
file(GLOB_RECURSE CORE_HEADER "./core/*.h")
file(GLOB_RECURSE ENGINE_SRC "./engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER "./engine/*.h")
file(GLOB_RECURSE GAME_SRC "./game/*.cpp")
file(GLOB_RECURSE GAME_ALL_HEADER "./game/*.h")
file(GLOB GAME_HEADER "./game/*.h")
file(GLOB GAME_PRIVATE "./game/_private/*.cpp")
file(GLOB GAME_ACTION_HEADER "./game/action/*.h")
file(GLOB GAME_ACTION_PRIVATE "./game/action/_private/*.cpp")
file(GLOB GAME_COMPONENT_HEADER "./game/component/*.h")
file(GLOB GAME_COMPONENT_PRIVATE "./game/component/_private/*.cpp")
file(GLOB GAME_SYSTEM_HEADER "./game/system/*.h")
file(GLOB GAME_SYSTEM_PRIVATE "./game/system/_private/*.cpp")

set(ALL_SRC main.cpp ${CORE_SRC} ${ENGINE_SRC} ${GAME_SRC})
set(ALL_HEADER ${GAME_ALL_HEADER} ${ENGINE_HEADER} ${CORE_HEADER})

include_directories(
		${PROJECT_BINARY_DIR}
		${OPENAL_INCLUDE_DIR}		
        ${PROJECT_SOURCE_DIR}/external/glfw/include/GLFW/
		${PROJECT_SOURCE_DIR}/external/glew/include/
		${PROJECT_SOURCE_DIR}/external/glm/
		${PROJECT_SOURCE_DIR}/external/bullet/src
		${PROJECT_SOURCE_DIR}/external/stb/
		${PROJECT_SOURCE_DIR}/core
		${PROJECT_SOURCE_DIR}/engine/_inc
		${PROJECT_SOURCE_DIR}/engine
		${PROJECT_SOURCE_DIR}/game
		${PROJECT_SOURCE_DIR}/game/action
		${PROJECT_SOURCE_DIR}/game/component
		${PROJECT_SOURCE_DIR}/game/system
		.)

set(ALL_LIBS
	${OPENGL_LIBRARIES}
	${OPENAL_LIBRARY}	
	glfw
	GLEW
	LinearMath
	BulletCollision
	BulletDynamics
	)

set(ALL_LIBS_EDITOR
  ${ALL_LIBS}
  #${wxWidgets_LIBRARIES}
  )

# Compile game
add_executable(tetrad-game ${ALL_SRC} ${ALL_HEADER})
target_link_libraries(tetrad-game ${ALL_LIBS})
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#set_target_properties(tetrad-game PROPERTIES COMPILE_FLAGS "--save-temps")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Compile package builder
#add_executable(packageBuilder
#${PROJECT_SOURCE_DIR}/tools/packageBuilder.cpp
#${CORE_SRC}
#${CORE_HEADER})
#target_link_libraries(packageBuilder ${ALL_LIBS_EDITOR})

# Compile package reader
#add_executable(packageReader
#${PROJECT_SOURCE_DIR}/tools/packageReader.cpp
#${CORE_SRC}
#${CORE_HEADER})
#target_link_libraries(packageReader ${ALL_LIBS})

## Setting up Visual Studio filters
source_group(Game\\_private FILES ${GAME_PRIVATE})
source_group(Game\\ FILES ${GAME_HEADER})
source_group(Game\\action\\_private FILES ${GAME_ACTION_PRIVATE})
source_group(Game\\action\\ FILES ${GAME_ACTION_HEADER})
source_group(Game\\component\\_private FILES ${GAME_COMPONENT_PRIVATE})
source_group(Game\\component\\ FILES ${GAME_COMPONENT_HEADER})
source_group(Game\\system\\_private FILES ${GAME_SYSTEM_PRIVATE})
source_group(Game\\system\\ FILES ${GAME_SYSTEM_HEADER})
source_group(Engine\\_private FILES ${ENGINE_SRC})
source_group(Engine\\ FILES ${ENGINE_HEADER})
source_group(Core\\_private FILES ${CORE_SRC})
source_group(Core\\ FILES ${CORE_HEADER})
