load('@rules_cc//cc:defs.bzl', 'cc_library')


config_setting(
    name = 'is_osx',
    values = {'cpu': 'darwin_x86_64'},
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
        ':is_osx': [],
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
        ':is_osx': [],
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
        ':is_osx': [],
        '//conditions:default': ['_GLFW_X11'],
    }),
    visibility = ['//visibility:public'],
)
