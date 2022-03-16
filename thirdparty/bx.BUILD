load('@rules_cc//cc:defs.bzl', 'cc_library')



cc_library(
    name = 'bx',
    srcs = glob(
        [
            'src/*.cpp',
        ],
        exclude=[
            'src/amalgamated.cpp',
            'src/crtnone.cpp',
        ],
    ),
    hdrs = glob([
        'include/bx/*.h',
        'include/bx/inline/*.inl',
        'include/tinystl/*.h',
        'src/*.h',
        '3rdparty/**/*.h',
    ]),
    includes = [
        'include',
        '3rdparty',
    ],
    defines = [
        '__STDC_FORMAT_MACROS',
        'BX_CONFIG_DEBUG=1',
    ],
    visibility = ['//visibility:public'],
)
