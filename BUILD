load('@rules_cc//cc:defs.bzl', 'cc_binary', 'cc_library')
load(
    '//:tools/shaderc.bzl',
    'shader_sources',
)



cc_library(
    name = 'effects',
    hdrs = glob([
        'src/**/*.h',
        ':shaders',
    ]),
    includes = [
        'src',
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

# https://stackoverflow.com/questions/38905256/bazel-copy-multiple-files-to-binary-directory
filegroup(
    name = 'tooling',
    srcs = select({
        '@bazel_tools//src/conditions:darwin': ['tools/darwin/shaderc'],
        '@bazel_tools//src/conditions:windows': ['tools/windows/shaderc'],
        '//conditions:default': ['tools/unix/shaderc'],
    }),
)

# py_binary(
#     name = 'shaderc',
#     srcs = ['tools/shaderc.py'],
# )

# genrule(
#     name = 'shaders',
#     srcs = glob([
#         'resources/shaders/**/*.sc',
#     ]),
#     outs = ['shaders.hpp'],
#     exec_tools = ['@bgfx//:bgfx_shader', ':shaderc', ':tooling'],
#     cmd = '$(location :shaderc) $(OUTS) $(location @bgfx//:bgfx_shader) $(SRCS)'
# )

shader_sources(
    name = 'shaders',
    shaders = glob([
        'resources/shaders/**/*.sc',
    ]),
    tooling = '//:tooling',
    bgfx_shader = '@bgfx//:bgfx_shader',
)
