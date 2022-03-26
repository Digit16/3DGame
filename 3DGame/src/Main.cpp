#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <fstream>
#include <sstream>

#include "Globals.h"
#include "Utils.h"
#include "Input.h"
#include "Shader.h"
#include "Renderer.h"

#define unpack3(a) a[0], a[1], a[2]
#define unpack4(a) a[0], a[1], a[2], a[3]


glm::vec3 bg(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPos(16.0, 0.0f, 0.0f);

class Game {
private:
    GLFWwindow* m_window;
    Shader* m_shader;

    unsigned int vertices_buffer;
    unsigned int color_buffer;
    unsigned int offset_buffer;
    unsigned int indices_buffer;

    double lastFrameTime;

public:
    Game() {
        Init();
        InitImGui();
    }

    ~Game() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void Run() {
        lastFrameTime = glfwGetTime();
        while (!glfwWindowShouldClose(m_window)) {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;


            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(unpack3(bg), 1.0f);

            Draw(deltaTime);
            DrawImGui(deltaTime);

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

private:
    void Init() {

        if (!glfwInit())
            std::cout << "Failed to init glfw!" << std::endl;

        m_window = glfwCreateWindow(Globals::windowWidth, Globals::windowHeight, "Hello World", NULL, NULL);
        if (!m_window) {
            glfwTerminate();
            std::cout << "Failed to create glfw window!" << std::endl;
        }

        glfwSetKeyCallback(m_window, Input::KeyboardCallback);
        glfwSetCursorPosCallback(m_window, Input::MousePositionCallback);

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);

        if (glewInit() != GLEW_OK)
            std::cout << "GLEW init error!" << std::endl;

        glEnable(GL_DEPTH_TEST);


        float vertices[] = {
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,

             1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f
        };
        float colors[] = {
            0.74f, 0.53f, 0.12f,
            0.63f, 0.35f, 0.39f,
            0.74f, 0.98f, 0.84f,
            0.12f, 0.57f, 0.89f,

            0.16f, 0.7f, 0.41f,
            0.75f, 0.16f, 0.033f,
            0.12f, 0.13f, 0.76f,
            0.78f, 0.53f, 0.64f,

            0.81f, 0.13f, 0.53f,
            0.0012f, 0.84f, 0.34f,
            0.96f, 0.96f, 0.95f,
            0.22f, 0.71f, 0.68f,

            0.055f, 0.58f, 1.0f,
            0.65f, 0.96f, 0.28f,
            0.38f, 0.19f, 0.85f,
            0.88f, 0.98f, 0.13f,

            0.7f, 0.63f, 0.69f,
            0.41f, 0.75f, 0.78f,
            0.12f, 0.87f, 0.73f,
            0.55f, 0.96f, 0.52f,

            0.77f, 0.89f, 0.18f,
            0.43f, 0.8f, 0.79f,
            0.84f, 0.7f, 0.45f,
            0.37f, 0.42f, 0.79f,
        };
        unsigned int indeces[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
        };

        vertices_buffer = Renderer::CreateBuffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        color_buffer = Renderer::CreateBuffer(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        offset_buffer = Renderer::CreateBuffer(GL_ARRAY_BUFFER, sizeof(float) * 3 * 100, nullptr, GL_DYNAMIC_DRAW);
        indices_buffer = Renderer::CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

        m_shader = new Shader("src/shaders/default");

        Renderer::UnbindAll();
    }
    void InitImGui() {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }
   
    void Draw(double deltaTime) {
 
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Globals::windowWidth / (float)Globals::windowHeight, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * model;
        m_shader->SetUniformMatrix4fv("u_MVP", mvp);

        glm::vec3 offsets[100];

        for (int x = 0; x < 10; ++x) {
            for (int z = 0; z < 10; ++z) {
                offsets[x * 10 + z] = glm::vec3(x * 2.0f - 10.0f, -5.0f, z * 2.0f - 10.0f);
            }
        }


        glBindBuffer(GL_ARRAY_BUFFER, offset_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0 * sizeof(float), 100 * 3 * sizeof(float), &offsets);

        // specify shader and VertexAttribArrays for draw call
        m_shader->Bind();

        glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, offset_buffer);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
        glVertexAttribDivisor(2, 1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);

        // draw call
        glDrawElementsInstanced(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr, 100);


    }
    void DrawImGui(double deltaTime) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("colors");
            ImGui::Text((char*)glGetString(GL_VERSION));
            ImGui::ColorEdit3("background", (float*)&bg, ImGuiColorEditFlags_NoInputs);
            ImGui::SliderFloat3("Proj", (float*)&cameraPos, -25.0f, 25.0f, "%.3f", 0);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

};


int main(void) {

    Game game;
    game.Run();

    return 0;
}