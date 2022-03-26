#include "Utils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

#include "Globals.h"

namespace Utils {
    glm::vec3 GetCameraRay(float xpos, float ypos, glm::mat4 projection, glm::mat4 view) {

        glm::vec4 rayClip = glm::vec4((2.0f * xpos) / Globals::windowWidth - 1.0f, 1.0f - (2.0f * ypos) / Globals::windowHeight, -1.0f, 1.0f);
        glm::vec4 rayEye = glm::inverse(projection) * rayClip;
        rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
        glm::vec3 rayWor = glm::normalize(glm::vec3(inverse(view) * rayEye));
        return rayWor;
    }

    const std::string ReadFile(const std::string filename) {
        std::ifstream file(filename);
        if (file.fail()) {
            std::cout << "Failed to open file!" << std::endl;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}