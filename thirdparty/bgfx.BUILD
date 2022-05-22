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
    name = 'meshoptimizer',
    srcs = glob([
        '3rdparty/meshoptimizer/src/*.cpp',
    ]),
    hdrs = ['3rdparty/meshoptimizer/src/meshoptimizer.h'],
    includes = ['3rdparty/meshoptimizer/src'],
)

cc_library(
    name = 'bgfx',
    srcs = glob(
        [
            'src/*.cpp',
        ],
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
        ':meshoptimizer',
        '@bimg//:bimg',
        '@bx//:bx',
    ] + select({
        '@bazel_tools//src/conditions:darwin': [':bgfx-objc'],
        '//conditions:default': [],
    }),
    copts = select({
        '@bazel_tools//src/conditions:darwin': [
            '-xobjective-c++',
        ],
        '//conditions:default': [],
    }),
    linkopts = select({
        '@bazel_tools//src/conditions:darwin': [
            '-F/Library/Frameworks',
            '-framework QuartzCore',
            '-framework Metal',
            '-framework Cocoa',
            '-framework IOKit',
            '-framework CoreVideo',
        ],
        '@bazel_tools//src/conditions:windows': [
            '-DEFAULTLIB:gdi32.lib',
            '-DEFAULTLIB:user32.lib',
            '-DEFAULTLIB:kernel32.lib',
            '-DEFAULTLIB:psapi.lib',
            '-DEFAULTLIB:d3d9.lib',
            '-DEFAULTLIB:d3d11.lib',
            '-DEFAULTLIB:d3d12.lib',
            '-DEFAULTLIB:dxgi.lib',
        ],
        '//conditions:default': [
            '-ldl',
            '-lGL',
            '-lpthread',
            '-lX11',
            '-lstdc++fs',
        ],
    }),
    visibility = ['//visibility:public'],
)

filegroup(
    name = 'bgfx_shader',
    srcs = ['src/bgfx_shader.sh'],
    visibility = ['//visibility:public'],
)
