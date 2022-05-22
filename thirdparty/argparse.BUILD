load('@rules_cc//cc:defs.bzl', 'cc_library')


cc_library(
    name = 'argparse',
    hdrs = glob([
        'include/argparse/*.hpp',
    ]),
    includes = [
        'include',
    ],
    copts = select({
        '@bazel_tools//src/conditions:windows': [
            '/std:c++17',
        ],
        '//conditions:default': [],
    }),
    visibility = ['//visibility:public'],
)
