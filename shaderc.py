import os
from pathlib import Path


shaderc = './bin/shaderc'
shader_src_dir = './resources/shaders/src/'
shader_bin_dir = './resources/shaders/bin'

shader_params = [
    ('glsl', 'linux', '440'),
    ('spirv', 'linux', 'spirv'),
    ('metal', 'osx', 'metal'),
    ('dx11', 'windows', '420'),
]


for shader_type in ('f', 'v'):
    for shader in Path(shader_src_dir).rglob(f'{shader_type}s_*'):
        for render_type_dir, platform, render_type in shader_params:
            outdir = f'{shader_bin_dir}/{render_type_dir}'

            Path(outdir).mkdir(parents=True, exist_ok=True)

            outpath = f'{outdir}/{shader.stem}.bin'
            print(outpath)

            command = f'{shaderc} -f {shader} -o {outpath} --type {shader_type} --platform {platform} -p {render_type} -i bazel-bgfx-bazel/external/bgfx/src --varyingdef ./resources/shaders/varying.def.sc'
            os.system(command)
