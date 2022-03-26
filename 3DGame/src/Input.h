#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Input {
    extern glm::vec2 mousePos;

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
}