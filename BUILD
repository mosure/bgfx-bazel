load('@rules_cc//cc:defs.bzl', 'cc_binary', 'cc_library')


cc_library(
    name = 'resources',
    hdrs = glob([
        'resources/**/*.h',
        'src/**/*.h',
    ]),
    includes = [
        'resources',
        'src',
    ],
)


cc_binary(
    name = 'example',
    srcs = [
        'src/main.cpp',
        'src/common/utils.cpp',
    ],
    deps = [
        ':resources',
        '@bgfx//:bgfx',
        '@glfw//:glfw',
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
)
