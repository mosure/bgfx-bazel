load('@rules_cc//cc:defs.bzl', 'cc_library')


cc_library(
    name = 'tinygltf',
    hdrs = [
        'stb_image.h',
        'stb_image_write.h',
        'tiny_gltf.h',
    ],
    deps = [
        '@json//:json',
    ],
    visibility = ['//visibility:public'],
)
