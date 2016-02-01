#pragma once

#include <string>

/** General Path Locations */
const std::string AssetPath = "../assets/";
const std::string TexturePath = AssetPath + "textures/";
const std::string ShaderPath = AssetPath + "shaders/";
const std::string FontPath = AssetPath + "fonts/";

/** Textures */
const std::string FLOOR_PATH = TexturePath + "Floor.tga";
const std::string BACKGROUND_PATH = TexturePath + "Background.tga";
const std::string PAUSE_BACKGROUND_PATH = TexturePath + "PauseGradient.tga";
const std::string ICON_PATH = TexturePath + "Icon.tga";

/** Fonts */
const std::string STANDARD_FONT_PATH = FontPath + "Standard.ttf";

/** Shaders */
const std::string TEST_VERTEX_PATH = ShaderPath + "testVert.glsl";
const std::string TEST_FRAG_PATH = ShaderPath + "testFrag.glsl";
