// Options to generate the Config.h file
#pragma once

#include <string>

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define SYSTEM_TYPE 2
#define ADDRESS_WIDTH 32

// Use full asset path for debug builds
//
// Production builds need to use a relative path so that executables
// can be deployed on machines without requiring a recompile
#if(BUILD_TYPE == 0)
const std::string ASSET_PATH = "E:/Study@UCLA/2017/Spring/CS130/CS130-Game/src/../assets/";
#else
const std::string ASSET_PATH = "../../assets/";
#endif
