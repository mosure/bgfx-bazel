load('@rules_cc//cc:defs.bzl', 'cc_library')


objc_library(
    name = 'bgfx-objc',
    non_arc_srcs = glob(
        [
            'src/*.mm',
        ],
        exclude=['src/amalgamated.mm'],
    ),
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
)

cc_library(
    name = 'bgfx',
    srcs = glob(
        ['src/*.cpp'],
        exclude=['src/amalgamated.cpp'],
    ),
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
    ] + select({
        '@bazel_tools//src/conditions:darwin': [':bgfx-objc'],
        '//conditions:default': [],
    }),
    defines = [
        'BX_CONFIG_DEBUG=0',
    ],
    copts = select({
        '@bazel_tools//src/conditions:darwin': [
            '-xobjective-c++',
        ],
        '//conditions:default': [],
    }),
    visibility = ['//visibility:public'],
)
