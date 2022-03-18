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

#define unpack3(a) a[0], a[1], a[2]
#define unpack4(a) a[0], a[1], a[2], a[3]

#define WIDTH 800
#define HEIGHT 600

glm::vec2 mousePos = {0.0f, 0.0f};


static const std::string ReadFile(const std::string filename) {
    std::ifstream file(filename);
    if (file.fail()) {
        std::cout << "Failed to open file!" << std::endl;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}



static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Shader error!" << std::endl;
        std::cout << message << std::endl;
        delete [] message;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexSharer, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSharer);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

// basename: filename without extension
static unsigned int CreateShaderFromFile(std::string basename) {
    std::string vertex_source = ReadFile(basename + ".vert");
    std::string fragment_source = ReadFile(basename + ".frag");
    unsigned int shader = CreateShader(vertex_source, fragment_source);
    return shader;
}

static unsigned int CreateBuffer(GLenum type, int size, const void* data, GLenum usage) {
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(type, buffer);
    glBufferData(type, size, data, usage);
    return buffer;
}

static void UnbindAll() {
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        std::cout << "key pressed: "<< scancode << std::endl;
}

static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << "mouse moved: (" << xpos << ", " << ypos << ")" << std::endl;
    mousePos = { xpos, ypos };
}

void MousePositionCallback(GLFWwindow* window, int button, int action, int mods)
{
    std::cout << "mouse button pressed: " << button << std::endl;
}

glm::vec3 GetCameraRay(float xpos, float ypos, glm::mat4 projection, glm::mat4 view) {

    glm::vec4 rayClip = glm::vec4((2.0f * xpos) / WIDTH - 1.0f, 1.0f - (2.0f * ypos) / HEIGHT, -1.0f, 1.0f);
    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    glm::vec3 rayWor = glm::normalize(glm::vec3(inverse(view) * rayEye));
    return rayWor;
}

int main(void) {
    GLFWwindow* window;

    
    if (!glfwInit())
        return -1;


    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }


    glfwSetKeyCallback(window, KeyboardCallback);
    glfwSetCursorPosCallback(window, MousePositionCallback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW init error!" << std::endl;
    
    glEnable(GL_DEPTH_TEST);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glm::vec3 bg(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraPos(16.0, 0.0f, 0.0f);



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
    float offsets[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 2.0f, 0.0f,
        0.0f, 4.0f, 0.0f,
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

    unsigned int vertices_buffer = CreateBuffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    unsigned int color_buffer    = CreateBuffer(GL_ARRAY_BUFFER, sizeof(colors)  , colors  , GL_STATIC_DRAW);
    unsigned int offset_buffer   = CreateBuffer(GL_ARRAY_BUFFER, sizeof(offsets) , offsets , GL_DYNAMIC_DRAW);

    unsigned int indices_buffer = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);


    unsigned int shader = CreateShaderFromFile("src/shaders/default");

    UnbindAll();
 
    unsigned int mvp_uniform_location = glGetUniformLocation(shader, "u_MVP");

    double lastFrameTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        std::cout << 1 / deltaTime << std::endl;

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
        


        // update 
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + glm::vec3(-8.0f, 0, 0.000001f), glm::vec3(0, 1, 0));
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = projection * view * model;
        glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, &mvp[0][0]);


        // move object to postion 10 units from camera
        glm::vec3 objPos = cameraPos + GetCameraRay(mousePos.x, mousePos.y, projection, view) * 10.0f;
        glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), 3 * sizeof(float), &objPos);


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(unpack3(bg), 1.0f);

        // specify shader and VertexAttribArrays for draw call
        glUseProgram(shader);

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
        glDrawElementsInstanced(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr, sizeof(offsets)/sizeof(offsets[0])/3);



        // render frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    //clear stuff
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();



    return 0;
}