
./bin/shaderc -f ./resources/shaders/fs_cubes.sc -o ./resources/shaders/glsl/fs_cubes.bin --type f --platform linux -p 440 -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
./bin/shaderc -f ./resources/shaders/vs_cubes.sc -o ./resources/shaders/glsl/vs_cubes.bin --type v --platform linux -p 440 -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc

./bin/shaderc -f ./resources/shaders/fs_cubes.sc -o ./resources/shaders/spirv/fs_cubes.bin --type f --platform linux -p spirv -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
./bin/shaderc -f ./resources/shaders/vs_cubes.sc -o ./resources/shaders/spirv/vs_cubes.bin --type v --platform linux -p spirv -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc

./bin/shaderc -f ./resources/shaders/fs_cubes.sc -o ./resources/shaders/metal/fs_cubes.bin --type f --platform osx -p metal -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
./bin/shaderc -f ./resources/shaders/vs_cubes.sc -o ./resources/shaders/metal/vs_cubes.bin --type v --platform osx -p metal -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc

./bin/shaderc -f ./resources/shaders/fs_cubes.sc -o ./resources/shaders/dx11/fs_cubes.bin --type f --platform windows -p 420 -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
./bin/shaderc -f ./resources/shaders/vs_cubes.sc -o ./resources/shaders/dx11/vs_cubes.bin --type v --platform windows -p 420 -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc
