def get_varying_def(source_file, varying_defs, default_def):
    for varying_def in varying_defs:
        if source_file.dirname == varying_def.dirname:
            return varying_def

    return default_def


def _shaderc_impl(ctx):
    shaderc_path = ctx.attr.tooling.files.to_list()[0].path
    print('Using shader tooling: ' + shaderc_path)

    common = [
        f for t in ctx.attr.common for f in t.files.to_list()
    ]

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
        outdir = f.dirname.split('resources/shaders')[1].strip('/')

        relative_path = name
        if outdir != '':
            relative_path = outdir + '/' + name

        shader_type = ''
        if '.fs' in name:
            shader_type = 'f'
            frags.append((name.replace('.fs', ''), relative_path, outdir))
        elif '.vs' in name:
            shader_type = 'v'
            verts.append((name.replace('.vs', ''), relative_path, outdir))

        for render_type_dir, platform, render_type in shader_params:
            outpath = 'resources/shaders/' + render_type_dir + '/' + outdir + '/' + name + '.bin'

            varying_def = get_varying_def(f, varying_defs, root_varying_def)

            out = ctx.actions.declare_file(outpath)
            runtime_files.append(out)

            ctx.actions.run_shell(
                outputs=[out],
                inputs=depset([
                    f,
                    varying_def,
                    bgfx_shader.files.to_list()[0],
                ] + common),
                tools=depset([
                    ctx.attr.tooling.files.to_list()[0],
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
                    bgfx_shader.files.to_list()[0].dirname,
                    '-i',
                    root_varying_def.dirname,
                    '--varyingdef',
                    varying_def.path,
                ]),
                progress_message='Compiling shader: ' + f.path,
                mnemonic = 'ShaderCompile'
            )

    frag_lines = []
    for program_name, outpath, outdir in frags:
        if outdir:
            frag_lines.append('namespace ' + outdir.replace('/', '::') + ' {')
            frag_lines.append('inline const char* const ' + program_name + ' = "' + outpath + '";')
            frag_lines.append('}')
        else:
            frag_lines.append('inline const char* const ' + program_name + ' = "' + outpath + '";')

    vert_lines = []
    for program_name, outpath, outdir in verts:
        if outdir:
            vert_lines.append('namespace ' + outdir.replace('/', '::') + ' {')
            vert_lines.append('inline const char* const ' + program_name + ' = "' + outpath + '";')
            vert_lines.append('}')
        else:
            vert_lines.append('inline const char* const ' + program_name + ' = "' + outpath + '";')

    generated_lines = [
        '#pragma once',
        '#include <string>',
        '// -------- autogenerated ---------',
        'namespace generated::shaders {',
        'namespace frag {'
    ] + frag_lines + [
        '} // namespace frag',
        'namespace vert {'
    ] + vert_lines + [
        '} // namespace vert',
        '} // namespace generated::shaders',
    ]

    ctx.actions.write(
        output = header_out,
        content = '\n'.join(generated_lines),
    )

    compile_ctx = cc_common.create_compilation_context(
        headers=depset([header_out]),
        includes=depset([header_out.dirname])
    )

    cc_info = CcInfo(compilation_context=compile_ctx)

    return [
        DefaultInfo(files = depset([header_out]), runfiles = ctx.runfiles(files=runtime_files)),
        cc_info
    ]


shader_sources = rule(
    implementation = _shaderc_impl,
    attrs = {
        'common': attr.label_list(allow_files=['.sh']),
        'shaders': attr.label_list(allow_files=['.sc']),
        'tooling': attr.label(allow_single_file=True),
        'bgfx_shader': attr.label(),
    },
)
