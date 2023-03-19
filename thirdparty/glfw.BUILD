load('@rules_cc//cc:defs.bzl', 'cc_library', 'objc_library')


objc_library(
    name = 'glfw-objc',
    non_arc_srcs = glob([
        'src/*.m',
    ]),
    hdrs = glob([
        'include/GLFW/*.h',
        'src/egl_context.h',
        'src/internal.h',
        'src/mappings.h',
        'src/osmesa_context.h',
        'src/nsgl_context.h',
        'src/cocoa_*.h',
        'src/null_joystick.h',
        'src/null_platform.h',
        'src/platform.h',
        'src/posix_*.h',
    ]),
    includes = [
        'include',
    ],
    defines = ['_GLFW_COCOA'],
)

cc_library(
    name = 'glfw',
    srcs = glob([
        'src/context.c',
        'src/egl_context.c',
        'src/init.c',
        'src/input.c',
        'src/monitor.c',
        'src/null_init.c',
        'src/null_joystick.c',
        'src/null_monitor.c',
        'src/null_window.c',
        'src/osmesa_context.c',
        'src/platform.c',
        'src/vulkan.c',
        'src/window.c',
    ]) + select({
        '@bazel_tools//src/conditions:darwin': glob([
            'src/cocoa_*.c',
            'src/posix*.c',
        ]),
        '@bazel_tools//src/conditions:windows': glob([
            'src/win32_*.c',
            'src/wgl_context.c',
        ]),
        '//conditions:default': glob([
            'src/glx_context.c',
            'src/linux*.c',
            'src/posix*.c',
            'src/x11*.c',
            'src/xkb*.c',
        ]),
    }),
    hdrs = glob([
        'include/GLFW/*.h',
        'src/egl_context.h',
        'src/internal.h',
        'src/mappings.h',
        'src/null_joystick.h',
        'src/null_platform.h',
        'src/osmesa_context.h',
        'src/platform.h',
    ]) + select({
        '@bazel_tools//src/conditions:darwin': glob([
            'src/cocoa_*.h',
            'src/posix_*.h',
        ]),
        '@bazel_tools//src/conditions:windows': glob([
            'src/win32_*.h',
            'src/wgl_context.h',
        ]),
        '//conditions:default': glob([
            'src/glx_context.h',
            'src/linux*.h',
            'src/posix*.h',
            'src/x11*.h',
            'src/xkb*.h',
        ]),
    }),
    includes = [
        'include',
    ],
    defines = []  + select({
        '@bazel_tools//src/conditions:darwin': ['_GLFW_COCOA'],
        '@bazel_tools//src/conditions:windows': ['_GLFW_WIN32'],
        '//conditions:default': ['_GLFW_X11'],
    }),
    deps = select({
        '@bazel_tools//src/conditions:darwin': [':glfw-objc'],
        '//conditions:default': [],
    }),
    visibility = ['//visibility:public'],
)
