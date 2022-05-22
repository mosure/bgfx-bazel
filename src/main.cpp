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
#include <GLFW/glfw3.h>
#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

#include "common/allocator.h"
#include "common/program.h"

#include "generated_textures.hpp"


static bool s_showStats = false;

static void glfw_errorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
        s_showStats = !s_showStats;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


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

    bool fullscreen = false;
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

    fullscreen = args_program.get<bool>("--fullscreen");
    program_name = args_program.get<std::string>("--program");


    // Create a GLFW window without an OpenGL context.
    glfwSetErrorCallback(glfw_errorCallback);
    if (!glfwInit())
        return 1;

    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    int div_scale = fullscreen ? 1 : 2;
    GLFWwindow *window = glfwCreateWindow(mode->width / div_scale, mode->height / div_scale, "helloworld", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (!window)
        return 1;

    glfwSetKeyCallback(window, glfw_keyCallback);

    // Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
    // Most graphics APIs must be used on the same thread that created the window.
    bgfx::renderFrame();

    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window);
#endif

    int width, height;
    glfwGetWindowSize(window, &width, &height);

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


    Uniforms m_uniforms;
    m_uniforms.init();

    example::PosVertex::init();


    std::unique_ptr<example::Program> active_program = get_program(program_name, width, height);


    static int64_t last = bx::getHPCounter();
    static float time = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);

        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, init.resolution.reset);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }
 
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

        m_uniforms.submit();
        active_program->submit(time);

        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }

    m_uniforms.destroy();
    active_program.reset();

    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
