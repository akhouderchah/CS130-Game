cmake_minimum_required(VERSION 2.6)
project(Iterate_2)

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
	set(CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG")
endif(MSVC)

configure_file(
	${PROJECT_SOURCE_DIR}/Config.h.in
	${PROJECT_BINARY_DIR}/Config.h )

set(CMAKE_CXX_STANDARD 11)

## Header and Source files
file(GLOB_RECURSE GAME_SRC "./game/*.cpp")
file(GLOB_RECURSE GAME_HEADER "./game/*.h")
file(GLOB_RECURSE CONTENT_SRC "./content/*.cpp")
file(GLOB_RECURSE CONTENT_HEADER "./content/*.h")

add_executable(Iterate_2
					${GAME_SRC}
					${GAME_HEADER}
					${CONTENT_SRC}
					${CONTENT_HEADER}
					main.cpp )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR}
		${PROJECT_BINARY_DIR}
		${PROJECT_SOURCE_DIR}
		${PROJECT_SOURCE_DIR}/game
		.)
	target_link_libraries(Iterate_2 ${SFML_LIBRARIES})
endif()

## Setting up Visual Studio filters
source_group(Game\\Source FILES ${GAME_SRC})
source_group(Game\\Headers FILES ${GAME_HEADER})
source_group(Content\\Source FILES ${CONTENT_SOURCE})
source_group(Content\\Headers FILES ${CONTENT_HEADER})

