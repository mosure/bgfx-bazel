#pragma once

#include <exception>
#include <iostream>
#include <initializer_list>
#include <string.h>
#include <vector>

#include <bgfx/bgfx.h>

#include "common/mesh.h"
#include "common/utils.h"

#include "generated_shaders.hpp"


namespace example {

struct PosVertex {
    float x;
    float y;
    float z;

    static void init() {
        ms_layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .end();
    }

    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosVertex::ms_layout;

static PosVertex planeVerticies[] = {
    {-1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f},
    {1.0f, -1.0f, 0.0f}
};

static const uint16_t planeTriList[] = {
    0, 1, 2,
    1, 3, 2
};


class Program {

public:
    virtual ~Program() = default;

    virtual void submit(float time) = 0;

};


class Program2d : public Program {

protected:
    bgfx::VertexBufferHandle vbh_;
    bgfx::IndexBufferHandle ibh_;

    bgfx::ProgramHandle program_;

public:
    Program2d(const std::string& shader_name) {
        vbh_ = bgfx::createVertexBuffer(
            bgfx::makeRef(planeVerticies, sizeof(planeVerticies)),
            PosVertex::ms_layout
        );

        ibh_ = bgfx::createIndexBuffer(
            bgfx::makeRef(planeTriList, sizeof(planeTriList))
        );

        program_ = loadProgram(generated::shaders::vert::program2d, shader_name + ".fs");
    }

    ~Program2d() {
        bgfx::destroy(ibh_);
        bgfx::destroy(vbh_);
        bgfx::destroy(program_);
    }

    void submit(float time) override {
        uint64_t state = 0
                | BGFX_STATE_WRITE_R
                | BGFX_STATE_WRITE_G
                | BGFX_STATE_WRITE_B
                | BGFX_STATE_PT_TRISTRIP;

        bgfx::setVertexBuffer(0, vbh_);
        bgfx::setIndexBuffer(ibh_);

        // Set render states.
        bgfx::setState(state);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, program_);
    }
};

class TextureProgram2d : public Program2d {

    struct TextureUniform {
        bgfx::TextureHandle texture;
        bgfx::UniformHandle uniform;
    };

protected:
    std::vector<TextureUniform> textures_;

public:
    TextureProgram2d(
        const std::string& shader_name,
        std::initializer_list<std::string> texture_names
    )
        : Program2d(shader_name)
    {
        for (auto& texture_name : texture_names) {
            textures_.push_back({
                loadTexture(texture_name),
                bgfx::createUniform(texture_name.c_str(),  bgfx::UniformType::Sampler)
            });
        }
    }

    ~TextureProgram2d() {
        for (auto& texture : textures_) {
            bgfx::destroy(texture.texture);
            bgfx::destroy(texture.uniform);
        }
    }

    void submit(float time) override {
        for (uint i = 0; i < textures_.size(); i++) {
            bgfx::setTexture(i, textures_[i].uniform, textures_[i].texture);
        }

        Program2d::submit(time);
    }
};


class MeshProgram : public Program {
protected:
    Mesh* mesh_;
    bgfx::ProgramHandle program_;

    int view_width_;
    int view_height_;

public:
    MeshProgram(
        const std::string& filepath,
        int view_width,
        int view_height
    )
        : view_width_(view_width)
        , view_height_(view_height)
    {
        program_ = loadProgram(generated::shaders::vert::mesh, generated::shaders::frag::mesh);

        mesh_ = meshLoad(getFilepath(filepath).c_str());
        if (!mesh_) {
            throw std::runtime_error("Failed to load mesh: " + getFilepath(filepath));
        }

        bgfx::setViewClear(0
            , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
            , 0x303030ff
            , 1.0f
            , 0
        );
    }

    ~MeshProgram() {
        meshUnload(mesh_);
    }

    void submit(float time) override {
        const bx::Vec3 at  = { 0.0f, 0.5f,  0.0f };
        const bx::Vec3 eye = { 0.0f, 1.0f, -2.5f };

        // Set view and projection matrix for view 0.
        {
            float view[16];
            bx::mtxLookAt(view, eye, at);

            float proj[16];
            bx::mtxProj(proj, 60.0f, float(view_width_)/float(view_height_), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
            bgfx::setViewTransform(0, view, proj);

            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, uint16_t(view_width_), uint16_t(view_height_) );
        }

        float mtx[16];
        bx::mtxRotateXY(mtx
            , 0.0f
            , time * 0.37f
        );

        meshSubmit(mesh_, 0, program_, mtx);
    }
};

} // namespace example
