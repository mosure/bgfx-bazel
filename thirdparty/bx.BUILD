load('@rules_cc//cc:defs.bzl', 'cc_library')


cc_library(
    name = 'bx',
    srcs = select({
        '@bazel_tools//src/conditions:darwin': glob(
            [
                'src/*.cpp',
            ],
            exclude=[
                'src/amalgamated.cpp',
                'src/crtnone.cpp',
            ],
        ),
        '//conditions:default': glob(
            [
                'src/*.cpp',
            ],
            exclude=[
                'src/amalgamated.cpp',
                'src/crtnone.cpp',
            ],
        ),
    }),
    hdrs = glob([
        'include/bx/*.h',
        'include/bx/inline/*.inl',
        'include/tinystl/*.h',
        'src/*.h',
        '3rdparty/**/*.h',
    ]) + select({
        '@bazel_tools//src/conditions:darwin': ['include/compat/osx/malloc.h'],
        '//conditions:default': [],
    }),
    includes = [
        'include',
        '3rdparty',
    ] + select({
        '@bazel_tools//src/conditions:darwin': ['include/compat/osx'],
        '//conditions:default': [],
    }),
    defines = [
        '__STDC_FORMAT_MACROS',
        'BX_CONFIG_DEBUG=1',
    ],
    visibility = ['//visibility:public'],
)
