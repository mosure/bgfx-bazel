#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include <bgfx/bgfx.h>
#include <bx/commandline.h>
#include <bx/endian.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include "common/utils.h"


static std::vector<std::shared_ptr<char[]>> loadedFiles;

static const bgfx::Memory* loadFile(const char* _filePath)
{
    auto filePath = std::filesystem::path(_filePath);

    if (filePath.is_relative()) {
        filePath = std::filesystem::current_path() / filePath;
        std::cout << _filePath << " is relative, actual path: " << filePath << std::endl;
    }

    if (std::filesystem::is_symlink(filePath))
    {
        filePath = std::filesystem::read_symlink(filePath);
        std::cout << _filePath << " is a symlink, actual path: " << filePath << std::endl;
    }

    std::ifstream is (filePath, std::ios::binary);
    if (!is) {
        std::cout << "Invalid filepath: " << _filePath << std::endl;
        return NULL;
    }

    is.seekg(0, std::ios::end);
    uint32_t size = is.tellg();
    is.seekg(0, std::ios::beg);

    std::shared_ptr<char[]> buffer { new char[size]};
    is.read(buffer.get(), size);

    loadedFiles.push_back(buffer);

    return bgfx::makeRef(buffer.get(), size);
}

bgfx::ShaderHandle loadShader(const char* _name)
{
    char filePath[512];

    const char* shaderPath = "???";

    switch (bgfx::getRendererType() )
    {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
    case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
    case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
    case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
    case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
    case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
    case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
    case bgfx::RendererType::WebGPU:     shaderPath = "shaders/spirv/"; break;

    case bgfx::RendererType::Count:
        BX_ASSERT(false, "You should not be here!");
        break;
    }

    bx::strCopy(filePath, BX_COUNTOF(filePath), "../../../../../resources/");
    bx::strCat(filePath, BX_COUNTOF(filePath), shaderPath);
    bx::strCat(filePath, BX_COUNTOF(filePath), _name);
    bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

    bgfx::ShaderHandle handle = bgfx::createShader(loadFile(filePath));
    bgfx::setName(handle, _name);

    return handle;
}

bgfx::ProgramHandle loadProgram(const char* _vsName, const char* _fsName)
{
    bgfx::ShaderHandle vsh = loadShader(_vsName);
    bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
    if (NULL != _fsName)
    {
        fsh = loadShader(_fsName);
    }

    return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}
