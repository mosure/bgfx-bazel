#pragma once

#include <string>

#include <bx/bx.h>
#include <bx/filepath.h>
#include <bx/string.h>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>



const std::string getFilepath(const std::string& _from_resources);

bgfx::ShaderHandle loadShader(const std::string& _name);
bgfx::ProgramHandle loadProgram(const std::string& _vsName, const std::string& _fsName);

bgfx::TextureHandle loadTexture(const std::string& _name, uint64_t _flags = BGFX_TEXTURE_NONE|BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL);
