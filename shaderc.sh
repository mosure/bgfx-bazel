
./bin/shaderc -f ./resources/shaders/fs_cubes.sc -o ./resources/shaders/glsl/fs_cubes.bin --type f --platform linux -p 440 -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
./bin/shaderc -f ./resources/shaders/vs_cubes.sc -o ./resources/shaders/glsl/vs_cubes.bin --type v --platform linux -p 440 -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
