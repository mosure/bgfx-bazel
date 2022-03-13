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
    commit = '7837d90865b391fd45698add941082823f5a83a6',
)

new_git_repository(
    name = 'bimg',
    remote = 'https://github.com/bkaradzic/bimg.git',
    build_file = '@//:thirdparty/bimg.BUILD',
    commit = 'bf830a85f5fc0e615aad445dbd7985031a6e754e',
)

new_git_repository(
    name = 'bgfx',
    remote = 'https://github.com/bkaradzic/bgfx.git',
    build_file = '@//:thirdparty/bgfx.BUILD',
    commit = 'e2eb3d56e6c43db6c531194d72e344817e6c4fb0',
)

new_git_repository(
    name = 'glfw',
    remote = 'https://github.com/glfw/glfw.git',
    build_file = '@//:thirdparty/glfw.BUILD',
    commit = '0b9e48fa3df9c184ff1abfb2452fd1a4b696ecd8',
)
