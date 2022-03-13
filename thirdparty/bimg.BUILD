load('@rules_cc//cc:defs.bzl', 'cc_library')



cc_library(
    name = 'bimg',
    srcs = glob([
        'src/image.cpp',
        'src/image_gnf.cpp',
        '3rdparty/iqa/source/*.c',
        '3rdparty/lodepng/lodepng.cpp',
        '3rdparty/tinyexr/deps/miniz/miniz.c',
    ]),
    hdrs = glob(
        [
            'src/*.h',
            'include/bimg/*.h',
            '3rdparty/**/*.h',
            '3rdparty/**/*.hpp',
        ],
        exclude=[
            '3rdparty/astc-codec/**/*',
        ],
    ),
    includes = [
        'include',
        '3rdparty',
        '3rdparty/lodepng',
        '3rdparty/tinyexr/deps/miniz',
        '3rdparty/iqa/include',
    ],
    deps = [
        '@bx//:bx',
        '@astc_codec//:astc_codec',
    ],
    visibility = ['//visibility:public'],
)
