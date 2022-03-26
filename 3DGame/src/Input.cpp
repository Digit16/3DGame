#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Input.h"

namespace Input {

    glm::vec2 mousePos = { 0.0f, 0.0f };

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
            std::cout << "key pressed: " << scancode << std::endl;
    }

    void MousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
        mousePos = { xpos, ypos };
    }

}