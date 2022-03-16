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
    linkopts = [
        '-ldl',
        '-lGL',
        '-lpthread',
        '-lX11',
    ],
)

