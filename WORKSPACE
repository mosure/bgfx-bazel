load('@bazel_tools//tools/build_defs/repo:git.bzl', 'git_repository', 'new_git_repository')
#load('@//conandeps:dependencies.bzl', 'load_conan_dependencies')
#load_conan_dependencies()



git_repository(
    name = 'astc_codec',
    remote = 'https://github.com/google/astc-codec.git',
    branch = 'master',
)


new_git_repository(
    name = 'bx',
    remote = 'https://github.com/bkaradzic/bx.git',
    build_file = '@//:thirdparty/bx.BUILD',
    commit = 'ad018d47c6c107e2fe2f3ba0716f9e728ed59a39',
    shallow_since = '1644116923 -0800',
)

new_git_repository(
    name = 'bimg',
    remote = 'https://github.com/bkaradzic/bimg.git',
    build_file = '@//:thirdparty/bimg.BUILD',
    commit = '663f724186e26caf46494e389ed82409106205fb',
)

new_git_repository(
    name = 'bgfx',
    remote = 'https://github.com/bkaradzic/bgfx.git',
    build_file = '@//:thirdparty/bgfx.BUILD',
    commit = '6a1809baa2ff1cb740db73cf23ad9df390a40ea3',
    shallow_since = '1647273539 -0700',
)

new_git_repository(
    name = 'glfw',
    remote = 'https://github.com/glfw/glfw.git',
    build_file = '@//:thirdparty/glfw.BUILD',
    commit = '0b9e48fa3df9c184ff1abfb2452fd1a4b696ecd8',
)
