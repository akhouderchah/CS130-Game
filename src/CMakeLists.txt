cmake_minimum_required(VERSION 2.6)
project(tetrad-game)

# Version Number
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

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

# 32 vs 64 bit
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ADDRESS_WIDTH 64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ADDRESS_WIDTH 32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME} ${ADDRESS_WIDTH}")

# Debug by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
  set(BUILD_TYPE 0)
else()
  set(BUILD_TYPE 1)
endif()

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

find_package(OpenGL REQUIRED)
add_subdirectory(external)

# Define _DEBUG on debug builds
if (MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-Og -D_DEBUG -Wall -g -ggdb -pedantic-errors -Wextra -lprofiler -fsanitize=address -fsanitize=leak -fsanitize=undefined")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -pedantic-errors -lprofiler -Wextra")
endif(MSVC)

configure_file(
	${PROJECT_SOURCE_DIR}/Config.h.in
	${PROJECT_BINARY_DIR}/Config.h )

set(CMAKE_CXX_STANDARD 11)

## Header and Source files
file(GLOB_RECURSE CORE_SRC "./core/*.cpp")
file(GLOB_RECURSE CORE_HEADER "./core/*.h")
file(GLOB_RECURSE ENGINE_SRC "./engine/*.cpp")
file(GLOB_RECURSE ENGINE_HEADER "./engine/*.h")
file(GLOB_RECURSE GAME_SRC "./game/*.cpp")
file(GLOB_RECURSE GAME_HEADER "./game/*.h")

set(ALL_SRC main.cpp ${CORE_SRC} ${ENGINE_SRC} ${GAME_SRC})
set(ALL_HEADER ${GAME_HEADER} ${ENGINE_HEADER} ${CORE_HEADER})

include_directories(
		${PROJECT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}/external/glfw/include/GLFW/
		${PROJECT_SOURCE_DIR}/external/glew/include/
		${PROJECT_SOURCE_DIR}/external/glm/
		${PROJECT_SOURCE_DIR}/external/soil/
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
	glfw
	GLEW
	SOIL
	)

add_executable(tetrad-game ${ALL_SRC})
target_link_libraries(tetrad-game ${ALL_LIBS})
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	#set_target_properties(tetrad-game PROPERTIES COMPILE_FLAGS "--save-temps")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

## Setting up Visual Studio filters
source_group(Game\\Source FILES ${GAME_SRC})
source_group(Game\\Headers FILES ${GAME_HEADER})
source_group(Engine\\Source FILES ${ENGINE_SRC})
source_group(Engine\\Headers FILES ${ENGINE_HEADER})
source_group(Core\\Source FILES ${CORE_SRC})
source_group(Core\\Headers FILES ${CORE_HEADER})

