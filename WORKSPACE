load('@bazel_tools//tools/build_defs/repo:git.bzl', 'git_repository', 'new_git_repository')
#load('@//conandeps:dependencies.bzl', 'load_conan_dependencies')
#load_conan_dependencies()



git_repository(
    name = 'astc_codec',
    remote = 'https://github.com/google/astc-codec.git',
    commit = '585aea744da3cb0beb07f0401982e2ace4285815',
)


new_git_repository(
    name = 'argparse',
    remote = 'https://github.com/p-ranav/argparse.git',
    build_file = '@//:thirdparty/argparse.BUILD',
    commit = 'c20621489d425145a6327715d7e3411a04f920a3',
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
    commit = '9a87635686c7fcb63ca63149c5b179b85a53a725',
)

new_git_repository(
    name = 'json',
    remote = 'https://github.com/nlohmann/json.git',
    tag = 'v3.10.5',
    build_file = '@//:thirdparty/json.BUILD',
)

new_git_repository(
    name = 'tinygltf',
    remote = 'https://github.com/syoyo/tinygltf.git',
    commit = '1a7c7d20013465b5c2a1285872b52a43ab62c1b3',
    build_file = '@//:thirdparty/tinygltf.BUILD',
)
