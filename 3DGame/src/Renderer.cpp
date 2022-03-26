
#include <GL/glew.h>


#include "Renderer.h"

namespace Renderer {

    unsigned int CreateBuffer(GLenum type, int size, const void* data, GLenum usage) {
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type, size, data, usage);
        return buffer;
    }

    void UnbindAll() {
        glUseProgram(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


}
