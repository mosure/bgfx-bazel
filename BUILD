load('@rules_cc//cc:defs.bzl', 'cc_binary', 'cc_library')
load(
    '//:tools/shaderc.bzl',
    'shader_sources',
)
load(
    '//:tools/texturec.bzl',
    'texture_sources',
)



cc_library(
    name = 'effects',
    hdrs = glob([
        'src/**/*.h',
    ]),
    includes = [
        'src',
    ],
    deps = [
        ':shaders',
        ':textures',
    ],
    copts = select({
        '@bazel_tools//src/conditions:windows': [
            '/std:c++17',
        ],
        '//conditions:default': [],
    }),
)


cc_binary(
    name = 'example',
    srcs = [
        'src/main.cpp',
        'src/common/allocator.cpp',
        'src/common/mesh.cpp',
        'src/common/utils.cpp',
    ],
    deps = [
        ':effects',
        '@argparse//:argparse',
        '@bgfx//:bgfx',
        '@glfw//:glfw',
        '@tinygltf//:tinygltf',
    ],
    linkopts = select({
        '@bazel_tools//src/conditions:darwin': [
            '-F/Library/Frameworks',
            '-framework QuartzCore',
            '-framework Metal',
            '-framework Cocoa',
            '-framework IOKit',
            '-framework CoreVideo',
        ],
        '//conditions:default': [
            '-ldl',
            '-lGL',
            '-lpthread',
            '-lX11',
            '-lstdc++fs',
        ],
    }),
    data = [
        ":shaders",
    ],
    copts = select({
        '@bazel_tools//src/conditions:windows': [
            '/std:c++17',
        ],
        '//conditions:default': [],
    }),
)

cc_binary(
    name = 'example-headless',
    srcs = [
        'src/main_headless.cpp',
        'src/common/allocator.cpp',
        'src/common/mesh.cpp',
        'src/common/utils.cpp',
    ],
    deps = [
        ':effects',
        '@argparse//:argparse',
        '@bgfx//:bgfx',
    ],
    linkopts = select({
        '@bazel_tools//src/conditions:darwin': [
            '-F/Library/Frameworks',
            '-framework QuartzCore',
            '-framework Metal',
            '-framework Cocoa',
            '-framework IOKit',
            '-framework CoreVideo',
        ],
        '//conditions:default': [
            '-ldl',
            '-lGL',
            '-lpthread',
            '-lX11',
            '-lstdc++fs',
        ],
    }),
    data = [
        ":shaders",
    ],
    copts = select({
        '@bazel_tools//src/conditions:windows': [
            '/std:c++17',
        ],
        '//conditions:default': [],
    }),
)

filegroup(
    name = 'shaderc',
    srcs = select({
        '@bazel_tools//src/conditions:darwin': ['tools/darwin/shaderc'],
        '@bazel_tools//src/conditions:windows': ['tools/windows/shaderc.exe'],
        '//conditions:default': ['tools/unix/shaderc'],
    }),
)

filegroup(
    name = 'texturec',
    srcs = select({
        '@bazel_tools//src/conditions:darwin': ['tools/darwin/texturec'],
        '@bazel_tools//src/conditions:windows': ['tools/windows/texturec'],
        '//conditions:default': ['tools/unix/texturec'],
    }),
)

shader_sources(
    name = 'shaders',
    shaders = glob([
        'resources/shaders/**/*.sc',
    ]),
    tooling = '//:shaderc',
    bgfx_shader = '@bgfx//:bgfx_shader',
)

texture_sources(
    name = 'textures',
    images = glob([
        'resources/textures/**/*.png',
    ]),
    tooling = '//:texturec',
)
