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

#include <chrono>

class Graphics {
public:
    Graphics();
    ~Graphics();

    const char* shaderSource = "src/Graphics/Shaders/Basic.shader";

    float pi = 3.14159f;
    
    GLFWwindow* window = NULL;
    unsigned int VBO = -1, VAO = -1, EBO = -1;
    unsigned int shaderProgram;

    int screenWidth = 1900;
    int screenHeight = 1040;
    float aspectRatio = (float)screenHeight / (float)screenWidth;

    int maxSightDistance = 10*1000;
    int minSightDistance = 0.05 * 1000;
    float fieldOfView = tan(pi*3/4);

    int init();
    void run();
    int cleanup();

    void multiplyMatVec(Vec3D& input, Vec3D& output, mat4x4& someMatrix);
    void multiplyMatVec(Vec3D& input, Vec3D& output, mat3x3& someMatrix);
};