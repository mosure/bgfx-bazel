#pragma once

#include <initializer_list>
#include <string.h>
#include <vector>

#include <bgfx/bgfx.h>

#include "common/utils.h"


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

    virtual void submit() = 0;

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
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(planeTriList, sizeof(planeTriList))
        );

        program_ = loadProgram("program2d.vs", shader_name + ".fs");
    }

    ~Program2d() {
        bgfx::destroy(ibh_);
        bgfx::destroy(vbh_);
        bgfx::destroy(program_);
    }

    void submit() override {
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

    void submit() override {
        for (uint i = 0; i < textures_.size(); i++) {
            bgfx::setTexture(i, textures_[i].uniform, textures_[i].texture);
        }

        Program2d::submit();
    }
};

}; // namespace example
