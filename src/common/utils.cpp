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
    std::ifstream is (_filePath, std::ios::binary);

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
    case bgfx::RendererType::Direct3D9:  shaderPath = "resources/shaders/dx9/";   break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderPath = "resources/shaders/dx11/";  break;
    case bgfx::RendererType::Gnm:        shaderPath = "resources/shaders/pssl/";  break;
    case bgfx::RendererType::Metal:      shaderPath = "resources/shaders/metal/"; break;
    case bgfx::RendererType::Nvn:        shaderPath = "resources/shaders/nvn/";   break;
    case bgfx::RendererType::OpenGL:     shaderPath = "resources/shaders/glsl/";  break;
    case bgfx::RendererType::OpenGLES:   shaderPath = "resources/shaders/essl/";  break;
    case bgfx::RendererType::Vulkan:     shaderPath = "resources/shaders/spirv/"; break;
    case bgfx::RendererType::WebGPU:     shaderPath = "resources/shaders/spirv/"; break;

    case bgfx::RendererType::Count:
        BX_ASSERT(false, "You should not be here!");
        break;
    }

    bx::strCopy(filePath, BX_COUNTOF(filePath), "/home/mosure/repos/bgfx-bazel/");
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
