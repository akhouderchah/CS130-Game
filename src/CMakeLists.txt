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

# Define _DEBUG on debug builds
if (MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG -Wall -Werror -g -ggdb -pedantic-errors -Wextra -fsanitize=address -fsanitize=leak -fsanitize=undefined")
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

add_executable(tetrad-game
					${SYSTEM_SRC}
					${GAME_SRC}
					${BASE_SRC}
					main.cpp )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR}
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}
		${PROJECT_SOURCE_DIR}/system
		${PROJECT_SOURCE_DIR}/base
		${PROJECT_SOURCE_DIR}/game
		.)
	target_link_libraries(tetrad-game ${SFML_LIBRARIES})
endif()

## Setting up Visual Studio filters
source_group(System\\Source FILES ${SYSTEM_SRC})
source_group(System\\Headers FILES ${SYSTEM_HEADER})
source_group(Game\\Source FILES ${GAME_SRC})
source_group(Game\\Headers FILES ${GAME_HEADER})
source_group(Base\\Source FILES ${BASE_SRC})
source_group(Base\\Headers FILES ${BASE_HEADER})
