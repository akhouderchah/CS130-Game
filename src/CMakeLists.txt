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

message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

# Debug by default
if(CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE Debug)
endif()

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

find_package(OpenGL REQUIRED)
add_subdirectory(external)

# Define _DEBUG on debug builds
if (MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -D_DEBUG -Wall -g -ggdb -pedantic-errors -Wextra -fsanitize=address -fsanitize=leak -fsanitize=undefined")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -pedantic-errors -Wextra")
endif(MSVC)

configure_file(
	${PROJECT_SOURCE_DIR}/Config.h.in
	${PROJECT_BINARY_DIR}/Config.h )

set(CMAKE_CXX_STANDARD 11)

## Header and Source files
file(GLOB_RECURSE SYSTEM_SRC "./system/*.cpp")
file(GLOB_RECURSE SYSTEM_HEADER "./system/*.h")
file(GLOB_RECURSE GAME_SRC "./game/*.cpp")
file(GLOB_RECURSE GAME_HEADER "./game/*.h")
file(GLOB_RECURSE BASE_SRC "./base/*.cpp")
file(GLOB_RECURSE BASE_HEADER "./base/*.h")
file(GLOB_RECURSE CORE_SRC "./core/*.cpp")
file(GLOB_RECURSE CORE_HEADER "./core/*.h")

set(ALL_SRC main.cpp ${SYSTEM_SRC} ${GAME_SRC} ${BASE_SRC} ${CORE_SRC})
set(ALL_HEADER ${SYSTEM_HEADER} ${GAME_HEADER} ${BASE_HEADER} ${CORE_HEADER})

include_directories(
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}/system
		${PROJECT_SOURCE_DIR}/base
		${PROJECT_SOURCE_DIR}/game
		${PROJECT_SOURCE_DIR}/core
		external/glfw/include/GLFW/
		external/glew/include/
		external/glm/
		.)

set(ALL_LIBS
	${OPENGL_LIBRARIES}
	glfw
	GLEW
	)

add_executable(tetrad-game ${ALL_SRC})
target_link_libraries(tetrad-game ${ALL_LIBS})

## Setting up Visual Studio filters
source_group(System\\Source FILES ${SYSTEM_SRC})
source_group(System\\Headers FILES ${SYSTEM_HEADER})
source_group(Game\\Source FILES ${GAME_SRC})
source_group(Game\\Headers FILES ${GAME_HEADER})
source_group(Base\\Source FILES ${BASE_SRC})
source_group(Core\\Headers FILES ${CORE_HEADER})
source_group(Core\\Source FILES ${CORE_SRC})

