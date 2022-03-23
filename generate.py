import os
from pathlib import Path
import platform


system = platform.system()

shaderc = {
    'Windows': 'tools/windows/shaderc',
    'Linux': 'tools/unix/shaderc',
    'Darwin': 'tools/darwin/shaderc',
}[system]
shader_src_dir = './resources/shaders/src/'
shader_bin_dir = './resources/shaders/bin'

shader_params = [
    ('glsl', 'linux', '440'),
    ('spirv', 'linux', 'spirv'),
    ('metal', 'osx', 'metal'),
    ('dx11', 'windows', '420'),
]

print('\nshaders:')
print('=' * 100)

for shader_type in ('f', 'v'):
    for shader in Path(shader_src_dir).rglob(f'*.{shader_type}s.sc'):
        print(shader.stem)

        for render_type_dir, platform, render_type in shader_params:
            outdir = f'{shader_bin_dir}/{render_type_dir}'

            Path(outdir).mkdir(parents=True, exist_ok=True)

            outpath = f'{outdir}/{shader.stem}.bin'

            command = f'{shaderc} -f {shader} -o {outpath} --type {shader_type} --platform {platform} -p {render_type} -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc'
            os.system(command)





texturec = {
    'Windows': 'tools/windows/texturec',
    'Linux': 'tools/unix/texturec',
    'Darwin': 'tools/darwin/texturec',
}[system]
texture_src_dir = './resources/textures/src/'
texture_bin_dir = './resources/textures/bin'

if os.path.exists(texturec):
    print('\ntextures:')
    print('=' * 100)

    for texture in Path(texture_src_dir).rglob(f'*.png'):
        print(texture.stem)

        outdir = f'{texture_bin_dir}'

        Path(outdir).mkdir(parents=True, exist_ok=True)

        outpath = f'{outdir}/{texture.stem}.dds'

        command = f'{texturec} -f {texture} -o {outpath}'
        os.system(command)
