#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
#include <iostream>

#include "Math/Triangle.h" 
#include "Math/Matrix.h"

class Graphics {
public:
    Graphics();
    ~Graphics();

    const char* vertexShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n\n"
        "layout (location = 0) in vec3 pos;\n\n"
        "void main() {\n"
        "   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
        "}\0";

    float pi = 3.14159f;
    
    GLFWwindow* window = NULL;
    unsigned int VBO = -1, VAO = -1, EBO = -1;
    unsigned int shaderProgram;

    int screenWidth = 1900;
    int screenHeight = 1040;
    float aspectRatio = (float)screenHeight / (float)screenWidth;

    int maxSightDistance = 1000*1000;
    int minSightDistance = 0.05 * 1000;
    float fieldOfView = tan(pi*3/4);

    int init();
    void run();
    int cleanup();

    void mutliplyMatVec(Vec3D& input, Vec3D& output, mat4x4& someMatrix);
};