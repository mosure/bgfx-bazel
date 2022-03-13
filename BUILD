load('@rules_cc//cc:defs.bzl', 'cc_binary', 'cc_library')


cc_library(
    name = 'resources',
    hdrs = glob([
        'resources/*.h',
    ]),
    includes = [
        'resources',
    ],
)


cc_binary(
    name = 'example',
    srcs = [
        'src/main.cpp',
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

