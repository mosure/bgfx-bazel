#pragma once

#include <string>

#include <bx/bx.h>
#include <bx/filepath.h>
#include <bx/string.h>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>



bgfx::ShaderHandle loadShader(const std::string& _name);
bgfx::ProgramHandle loadProgram(const std::string& _vsName, const std::string& _fsName);
