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
        'src/posix_thread.h',
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
        'src/osmesa_context.c',
        'src/vulkan.c',
        'src/window.c',
    ]) + select({
        '@bazel_tools//src/conditions:darwin': glob([
            'src/cocoa_*.c',
            'src/posix_thread.c',
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
        'src/osmesa_context.h',
    ]) + select({
        '@bazel_tools//src/conditions:darwin': glob([
            'src/cocoa_*.h',
            'src/posix_thread.h',
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
        '//conditions:default': ['_GLFW_X11'],
    }),
    deps = select({
        '@bazel_tools//src/conditions:darwin': [':glfw-objc'],
        '//conditions:default': [],
    }),
    visibility = ['//visibility:public'],
)
