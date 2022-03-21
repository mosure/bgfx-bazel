#include <iostream>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include <argparse/argparse.hpp>

#include <bx/bx.h>
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

#include "common/utils.h"


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


struct PosVertex {
    float x;
    float y;
    float z;

    static void init() {
        ms_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosVertex::ms_layout;


static PosVertex planeVerticies[] = {
    {-1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 0.0f}
};

static const uint16_t planeTriList[] = {
    0, 1, 2,
    1, 3, 2
};


int main(int argc, char **argv)
{
    bool fullscreen = false;
    std::string program_name = "cubes";

    argparse::ArgumentParser args_program("example", "1.0.0");
    args_program.add_argument("-f", "--fullscreen")
        .help("whether or not to run in fullscreen")
        .default_value(false)
        .implicit_value(true);

    args_program.add_argument("-p", "--program")
        .default_value(std::string("cubes"))
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


    uint64_t state = 0
                | BGFX_STATE_WRITE_R
                | BGFX_STATE_WRITE_G
                | BGFX_STATE_WRITE_B
                | BGFX_STATE_PT_TRISTRIP;

    PosVertex::init();

    bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(planeVerticies, sizeof(planeVerticies)),
        PosVertex::ms_layout
    );

    bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(planeTriList, sizeof(planeTriList))
    );

    bgfx::ProgramHandle program = loadProgram(program_name + ".vs", program_name + ".fs");

    static int64_t last = bx::getHPCounter();
    static float time = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Handle window resize.
        int oldWidth = width, oldHeight = height;
        glfwGetWindowSize(window, &width, &height);

        if (width != oldWidth || height != oldHeight) {
            bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
            bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }


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


        // This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
        //bgfx::touch(kClearView);

        // Use debug font to print information about this example.
        // bgfx::dbgTextClear();
        // bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
        // bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
        // bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
        // bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
        // bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");

        // const bgfx::Stats* stats = bgfx::getStats();
        // bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);

        // Enable stats or debug text.
        bgfx::setDebug(s_showStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_NONE);

        {
            m_uniforms.submit();

            bgfx::setVertexBuffer(0, vbh);
            bgfx::setIndexBuffer(ibh);

            // Set render states.
            bgfx::setState(state);

            // Submit primitive for rendering to view 0.
            bgfx::submit(0, program);
            //bgfx::discard();
        }

        // Advance to next frame. Process submitted rendering primitives.
        bgfx::frame();
    }

    m_uniforms.destroy();
    bgfx::destroy(ibh);
    bgfx::destroy(vbh);
    bgfx::destroy(program);

    bgfx::shutdown();
    glfwTerminate();
    return 0;
}
