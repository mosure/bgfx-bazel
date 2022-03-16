#pragma once

#include <bx/bx.h>
#include <bx/filepath.h>
#include <bx/string.h>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>



bgfx::ShaderHandle loadShader(const char* _name);
bgfx::ProgramHandle loadProgram(const char* _vsName, const char* _fsName);
