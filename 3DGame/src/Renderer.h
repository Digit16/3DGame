#pragma once

namespace Renderer {
    unsigned int CreateBuffer(GLenum type, int size, const void* data, GLenum usage);
    void UnbindAll();
}