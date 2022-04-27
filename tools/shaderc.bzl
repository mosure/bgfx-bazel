

def get_varying_def(source_file, varying_defs, default_def):
    for varying_def in varying_defs:
        if source_file.dirname == varying_def.dirname:
            return varying_def

    return default_def


def _shaderc_impl(ctx):
    shaderc_path = ctx.attr.tooling.files.to_list()[0].path
    print('Using shader tooling: ' + shaderc_path)


    shaders = [
        f for t in ctx.attr.shaders for f in t.files.to_list()
    ]
    bgfx_shader = ctx.attr.bgfx_shader

    runtime_files = []
    header_out = ctx.actions.declare_file('generated_shaders.hpp')


    frags_and_verts = []
    varying_defs = []
    root_varying_def = None
    frags = []
    verts = []

    for f in shaders:
        if 'varying.def.sc' in f.short_path:
            varying_defs.append(f)

            if root_varying_def == None or len(root_varying_def.short_path) > len(f.short_path):
                root_varying_def = f
        else:
            frags_and_verts.append(f)


    shader_params = [
        ('glsl', 'linux', '440'),
        ('spirv', 'linux', 'spirv'),
        ('metal', 'osx', 'metal'),
        ('dx11', 'windows', '420'),
    ]


    for f in frags_and_verts:
        name = f.basename.replace('.sc', '')

        for render_type_dir, platform, render_type in shader_params:
            outdir = f.dirname.replace('resources/shaders/', '/').replace('resources/shaders', '')
            outpath = 'resources/shaders/' + render_type_dir + outdir + '/' + name + '.bin'

            print(outpath)

            varying_def = get_varying_def(f, varying_defs, root_varying_def)


            shader_type = ''
            if '.fs' in name:
                shader_type = 'f'
                frags.append((name.replace('.fs', ''), outpath))
            elif '.vs' in name:
                shader_type = 'v'
                verts.append((name.replace('.vs', ''), outpath))


            out = ctx.actions.declare_file(outpath)
            runtime_files.append(out)


            ctx.actions.run_shell(
                outputs=[out],
                inputs=depset([
                    ctx.attr.tooling.files.to_list()[0],
                    f,
                    varying_def,
                    bgfx_shader.files.to_list()[0]
                ]),
                command=shaderc_path + ' '.join([
                    ' -f',
                    f.path,
                    '-o',
                    out.path,
                    '--type',
                    shader_type,
                    '--platform',
                    platform,
                    '-p',
                    render_type,
                    '-i',
                    bgfx_shader.files.to_list()[0].path,
                    '--varyingdef',
                    varying_def.path,
                ]),
                progress_message='Compiling shader: ' + f.path,
                mnemonic = 'ShaderCompile'
            )

    ctx.actions.write(
        output = header_out,
        content = "Hello\n",
    )

    return [
        DefaultInfo(files = depset([header_out]), runfiles = ctx.runfiles(files=runtime_files))
    ]


shader_sources = rule(
    implementation = _shaderc_impl,
    attrs = {
        'shaders': attr.label_list(allow_files=['.sc']),
        'tooling': attr.label(allow_single_file=True),
        'bgfx_shader': attr.label(),
    },
)
