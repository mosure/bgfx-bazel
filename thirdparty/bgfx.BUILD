load('@rules_cc//cc:defs.bzl', 'cc_library')



config_setting(
    name = 'is_osx',
    values = {'cpu': 'darwin_x86_64'},
)

cc_library(
    name = 'bgfx',
    srcs = select({
        ':is_osx': ['src/amalgamated.mm'],
        '//conditions:default': glob(
            ['src/*.cpp'],
            exclude=['src/amalgamated.cpp'],
        ),
    }),
    hdrs = glob([
        'include/bgfx/**/*.h',
        'src/*.h',
        'src/*.inl',
        '3rdparty/**/*.h',
    ]),
    includes = [
        'include',
        '3rdparty',
        '3rdparty/dxsdk/include',
        '3rdparty/khronos',
        '3rdparty/iqa/include',
    ],
    deps = [
        '@bimg//:bimg',
        '@bx//:bx',
    ],
    defines = [
        'BGFX_CONFIG_DEBUG=0',
    ],
    visibility = ['//visibility:public'],
)
