#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include <argparse/argparse.hpp>

#include <bx/bx.h>
#include <bx/bounds.h>
#include <bx/file.h>
#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "common/allocator.h"
#include "common/program.h"

#include "generated_textures.hpp"


static bool s_showStats = false;


struct Uniforms {
    void init() {
        u_times = bgfx::createUniform("u_times", bgfx::UniformType::Vec4, 1);
    }

    void submit() {
        bgfx::setUniform(u_times, m_times, 1);
    }

    void destroy()
    {
        bgfx::destroy(u_times);
    }

    union {
        struct {
            float m_delta_seconds;
            float m_time;
        };

        float m_times[4];
    };

    bgfx::UniformHandle u_times;
};


static std::unique_ptr<example::Program> get_program(const std::string& program_name, int width, int height) {
    if (program_name == "logo") {
        return std::make_unique<example::TextureProgram2d>(
            generated::shaders::frag::logo,
            std::initializer_list<std::string>({ generated::textures::s_logo })
        );
    } else if (program_name == "mask") {
        return std::make_unique<example::TextureProgram2d>(
            generated::shaders::frag::mask,
            std::initializer_list<std::string>({
                generated::textures::s_dog,
                generated::textures::s_dog_mask
            })
        );
    } else if (program_name == "mesh") {
        return std::make_unique<example::MeshProgram>("models/bunny.bgfx.bin", width, height);
    } else {
        return std::make_unique<example::Program2d>(program_name);
    }
}



int main(int argc, char **argv)
{
    example::alloc_init();

    std::string program_name = generated::shaders::frag::cubes;

    argparse::ArgumentParser args_program("example", "1.0.0");
    args_program.add_argument("-f", "--fullscreen")
        .help("whether or not to run in fullscreen")
        .default_value(false)
        .implicit_value(true);

    args_program.add_argument("-p", "--program")
        .default_value(std::string(generated::shaders::frag::cubes))
        .help("name of program to run");

    try {
        args_program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << args_program;
        std::exit(1);
    }

    program_name = args_program.get<std::string>("--program");


    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;

    int width = 1920;
    int height = 1080;

    init.resolution.width = (uint32_t)width;
    init.resolution.height = (uint32_t)height;
    init.resolution.reset = BGFX_RESET_MSAA_X16 | BGFX_RESET_VSYNC; // BGFX_RESET_NONE, BGFX_RESET_MSAA_X16, BGFX_RESET_VSYNC

    if (!bgfx::init(init))
        return 1;

    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    const bgfx::ViewId kClearView = 0;
    bgfx::setViewClear(kClearView, BGFX_CLEAR_COLOR);
    bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

    bgfx::setDebug(BGFX_DEBUG_NONE);

    const bgfx::Caps* caps = bgfx::getCaps();

    if (caps->supported && BGFX_CAPS_TEXTURE_DIRECT_ACCESS) {
        std::cout << "Texture direct access supported!" << std::endl;
    } else {
        std::cout << "Texture direct access is NOT supported." << std::endl;
    }

    if (caps->supported && BGFX_CAPS_TEXTURE_READ_BACK) {
        std::cout << "Texture read back supported!" << std::endl;
    } else {
        std::cout << "Texture read back is NOT supported." << std::endl;
    }

    if (caps->supported && BGFX_CAPS_TEXTURE_BLIT) {
        std::cout << "Texture blit is supported!" << std::endl;
    } else {
        std::cout << "Texture blit is NOT supported." << std::endl;
    }


    Uniforms m_uniforms;
    m_uniforms.init();

    example::PosVertex::init();

    bgfx::FrameBufferHandle target_fb = bgfx::createFrameBuffer(
        width,
        height,
        bgfx::TextureFormat::BGRA8
    );

    uint64_t flags = BGFX_TEXTURE_BLIT_DST | BGFX_TEXTURE_READ_BACK;
    bgfx::TextureHandle dst_texture = bgfx::createTexture2D(
        width,
        height,
        false,
        1,
        bgfx::TextureFormat::BGRA8,
        flags
    );

    std::unique_ptr<example::Program> active_program = get_program(program_name, width, height);


    static int64_t last = bx::getHPCounter();
    static float time = 0.0f;


    // set global uniforms
    {
        int64_t now = bx::getHPCounter();
        const int64_t frameTime = now - last;
        last = now;
        const double freq = double(bx::getHPFrequency() );
        const float deltaTimeSec = float(double(frameTime)/freq);
        time += deltaTimeSec;

        m_uniforms.m_delta_seconds = deltaTimeSec;
        m_uniforms.m_time = time;
    }

    // Enable stats or debug text.
    bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_NONE);

    bgfx::setViewFrameBuffer(0, target_fb);

    std::cout << "About to process frame" << std::endl;

    m_uniforms.submit();
    active_program->submit(time);

    // Advance to next frame. Process submitted rendering primitives.
    std::cout << "Current frame: " << bgfx::frame() << std::endl;

    std::cout << "Post frame extraction" << std::endl;
    std::cout << std::flush;

    // blit target -> dst
    bgfx::blit(
        0,
        dst_texture,
        0,
        0,
        bgfx::getTexture(target_fb),
        0,
        0,
        width,
        height
    );


    // extract frame
    void* direct_access_ptr = bgfx::getDirectAccessPtr(bgfx::getTexture(target_fb));
    if (direct_access_ptr == nullptr) {
        std::cout << "Direct access ptr is NULL" << std::endl;
    } else if ((uint64_t)direct_access_ptr == UINTPTR_MAX) {
        std::cout << "Direct access ptr texture is pending creation..." << std::endl;
    } else {
        std::cout << "Direct access ptr: " << direct_access_ptr << std::endl;
    }

    bgfx::TextureInfo tex_info;
    bgfx::calcTextureSize(tex_info, width, height, 4, false, false, 1, bgfx::TextureFormat::BGRA8);
    void* data_ptr = (void*) malloc(tex_info.storageSize);


    uint32_t available_frame = bgfx::readTexture(dst_texture, data_ptr);
    std::cout << "Texture read back content will be available at frame: " << available_frame << std::endl;

    std::cout << "First byte: " << *((uint8_t*)data_ptr) << std::endl;

    std::cout << "Current frame: " << bgfx::frame() << std::endl;
    std::cout << "Current frame: " << bgfx::frame() << std::endl;

    std::cout << "First byte: " << *((uint8_t*)data_ptr) << std::endl;

    std::ofstream outfile("out.raw", std::ios::binary);
    outfile.write((char*)data_ptr, tex_info.storageSize);
    outfile.close();


    m_uniforms.destroy();
    active_program.reset();

    bgfx::destroy(target_fb);
    bgfx::shutdown();
    return 0;
}
