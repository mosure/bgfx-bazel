load('@rules_cc//cc:defs.bzl', 'cc_library')


cc_library(
    name = 'json',
    hdrs = glob(
        ['single_include/nlohmann/*.hpp'],
    ),
    includes = ['single_include/nlohmann'],
    strip_include_prefix = 'single_include/nlohmann',
    visibility = ['//visibility:public'],
)
