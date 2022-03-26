#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Utils {
    glm::vec3 GetCameraRay(float xpos, float ypos, glm::mat4 projection, glm::mat4 view);
    const std::string ReadFile(const std::string filename);
}