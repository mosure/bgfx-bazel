load('@rules_cc//cc:defs.bzl', 'cc_binary', 'cc_library')
load(
    '//:tools/shaderc.bzl',
    'shader_sources',
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
    ],
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
    copts = [
        '-std=c++17',
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
)

filegroup(
    name = 'tooling',
    srcs = select({
        '@bazel_tools//src/conditions:darwin': ['tools/darwin/shaderc'],
        '@bazel_tools//src/conditions:windows': ['tools/windows/shaderc'],
        '//conditions:default': ['tools/unix/shaderc'],
    }),
)

shader_sources(
    name = 'shaders',
    shaders = glob([
        'resources/shaders/**/*.sc',
    ]),
    tooling = '//:tooling',
    bgfx_shader = '@bgfx//:bgfx_shader',
)
