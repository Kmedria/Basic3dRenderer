#include "Graphics.h"
#include <filesystem>

Graphics::Graphics() {};
Graphics::~Graphics() {};

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);

int Graphics::init() {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialise GLFW.\n");
        return -1;
    }

    //glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(screenWidth, screenHeight, "Mind your own business", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Initialize 

    //glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
    };

    return 0;
};

void Graphics::run() {
    
    
    std::ifstream stream;

    stream.open(shaderSource);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else {
                if (line.find("fragment") != std::string::npos) {
                    type = ShaderType::FRAGMENT;
                }
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    const std::string& vertSTR = ss[0].str();
    const char* vert = vertSTR.c_str();

    glShaderSource(vertexShader, 1, &vert, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    const std::string& fragSTR = ss[1].str();
    const char* frag = fragSTR.c_str();
    
    glShaderSource(fragmentShader, 1, &frag, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    mat4x4 projectionMat;
    for (int index = 0; index < 4; index++) {
        for (int index2 = 0; index < 4; index++) {
            projectionMat.data[index][index2] = 0;
        }
    }

    /*
    projectionMat.data[0][0] = (aspectRatio * fieldOfView);
    projectionMat.data[1][1] = fieldOfView;
    projectionMat.data[2][2] = maxSightDistance / (maxSightDistance - minSightDistance);
    projectionMat.data[3][2] = -(float)minSightDistance * projectionMat.data[2][2];
    projectionMat.data[2][3] = 1;
    projectionMat.data[3][3] = 0;
    */
    
    float x = -500, y = -500, z = 100;

    projectionMat.data[0][0] = aspectRatio * fieldOfView;
    projectionMat.data[1][1] = fieldOfView;
    projectionMat.data[2][2] = maxSightDistance / (maxSightDistance - minSightDistance);
    projectionMat.data[2][3] = 1;
    projectionMat.data[3][0] = x * aspectRatio * fieldOfView;
    projectionMat.data[3][1] = y * fieldOfView;
    projectionMat.data[3][2] = z * (maxSightDistance / (maxSightDistance - minSightDistance)) - ((float)minSightDistance * maxSightDistance) / (maxSightDistance - minSightDistance);
    projectionMat.data[3][3] = z;
    
    Vec3D someTriangle0 = Vec3D(1000.0f, 1000.0f, 1000.0f);
    Vec3D someTriangle1 = Vec3D(1000.0f, 1000.0f, 0.0f);
    Vec3D someTriangle2 = Vec3D(1000.0f, 0.0f, 1000.0f);
    Vec3D someTriangle3 = Vec3D(1000.0f, 0.0f, 0.0f);
    Vec3D someTriangle4 = Vec3D(0.0f, 1000.0f, 1000.0f);
    Vec3D someTriangle5 = Vec3D(0.0f, 1000.0f, 0.0f);
    Vec3D someTriangle6 = Vec3D(0.0f, 0.0f, 1000.0f);
    Vec3D someTriangle7 = Vec3D(0.0f, 0.0f, 0.0f);

    Vec3D someTriangle8 = Vec3D(1000.0f, 1000.0f, 2000.0f);
    Vec3D someTriangle9 = Vec3D(1000.0f, 1000.0f, 1000.0f);
    Vec3D someTriangle10 = Vec3D(1000.0f, 0.0f, 2000.0f);
    Vec3D someTriangle11 = Vec3D(1000.0f, 0.0f, 1000.0f);
    Vec3D someTriangle12 = Vec3D(0.0f, 1000.0f, 2000.0f);
    Vec3D someTriangle13 = Vec3D(0.0f, 1000.0f, 1000.0f);
    Vec3D someTriangle14 = Vec3D(0.0f, 0.0f, 2000.0f);
    Vec3D someTriangle15 = Vec3D(0.0f, 0.0f, 1000.0f);
    
    Vec3D someTriangle16 = Vec3D(1000.0f, 1000.0f, 3000.0f);
    Vec3D someTriangle17 = Vec3D(1000.0f, 1000.0f, 2000.0f);
    Vec3D someTriangle18 = Vec3D(1000.0f, 0.0f, 3000.0f);
    Vec3D someTriangle19 = Vec3D(1000.0f, 0.0f, 2000.0f);
    Vec3D someTriangle20 = Vec3D(0.0f, 1000.0f, 3000.0f);
    Vec3D someTriangle21 = Vec3D(0.0f, 1000.0f, 2000.0f);
    Vec3D someTriangle22 = Vec3D(0.0f, 0.0f, 3000.0f);
    Vec3D someTriangle23 = Vec3D(0.0f, 0.0f, 2000.0f);

    Vec3D newTriangle0;
    Vec3D newTriangle1;
    Vec3D newTriangle2;
    Vec3D newTriangle3;
    Vec3D newTriangle4;
    Vec3D newTriangle5;
    Vec3D newTriangle6;
    Vec3D newTriangle7;

    Vec3D newTriangle8;
    Vec3D newTriangle9;
    Vec3D newTriangle10;
    Vec3D newTriangle11;
    Vec3D newTriangle12;
    Vec3D newTriangle13;
    Vec3D newTriangle14;
    Vec3D newTriangle15;
    
    Vec3D newTriangle16;
    Vec3D newTriangle17;
    Vec3D newTriangle18;
    Vec3D newTriangle19;
    Vec3D newTriangle20;
    Vec3D newTriangle21;
    Vec3D newTriangle22;
    Vec3D newTriangle23;

    mutliplyMatVec(someTriangle0, newTriangle0, projectionMat);
    mutliplyMatVec(someTriangle1, newTriangle1, projectionMat);
    mutliplyMatVec(someTriangle2, newTriangle2, projectionMat);
    mutliplyMatVec(someTriangle3, newTriangle3, projectionMat);
    mutliplyMatVec(someTriangle4, newTriangle4, projectionMat);
    mutliplyMatVec(someTriangle5, newTriangle5, projectionMat);
    mutliplyMatVec(someTriangle6, newTriangle6, projectionMat);
    mutliplyMatVec(someTriangle7, newTriangle7, projectionMat);

    mutliplyMatVec(someTriangle8, newTriangle8, projectionMat);
    mutliplyMatVec(someTriangle9, newTriangle9, projectionMat);
    mutliplyMatVec(someTriangle12, newTriangle12, projectionMat);
    mutliplyMatVec(someTriangle13, newTriangle13, projectionMat);
    mutliplyMatVec(someTriangle14, newTriangle14, projectionMat);
    mutliplyMatVec(someTriangle15, newTriangle15, projectionMat);
    mutliplyMatVec(someTriangle10, newTriangle10, projectionMat);
    mutliplyMatVec(someTriangle11, newTriangle11, projectionMat);

    mutliplyMatVec(someTriangle16, newTriangle16, projectionMat);
    mutliplyMatVec(someTriangle17, newTriangle17, projectionMat);
    mutliplyMatVec(someTriangle18, newTriangle18, projectionMat);
    mutliplyMatVec(someTriangle19, newTriangle19, projectionMat);
    mutliplyMatVec(someTriangle20, newTriangle20, projectionMat);
    mutliplyMatVec(someTriangle21, newTriangle21, projectionMat);
    mutliplyMatVec(someTriangle22, newTriangle22, projectionMat);
    mutliplyMatVec(someTriangle23, newTriangle23, projectionMat);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------ 

    float vertices[] = {
        // 0.5f,  0.5f, 0.0f,  // top right
        // 0.5f, -0.5f, 0.0f,  // bottom right
        //-0.5f, -0.5f, 0.0f,  // bottom left
        //-0.5f,  0.5f, 0.0f   // top left 
        newTriangle0.x, newTriangle0.y, newTriangle0.z,
        newTriangle1.x, newTriangle1.y, newTriangle1.z,
        newTriangle2.x, newTriangle2.y, newTriangle2.z,
        newTriangle3.x, newTriangle3.y, newTriangle3.z,
        newTriangle4.x, newTriangle4.y, newTriangle4.z,
        newTriangle5.x, newTriangle5.y, newTriangle5.z,
        newTriangle6.x, newTriangle6.y, newTriangle6.z,
        newTriangle7.x, newTriangle7.y, newTriangle7.z,
        newTriangle8.x, newTriangle8.y, newTriangle8.z,
        newTriangle9.x, newTriangle9.y, newTriangle9.z,
        newTriangle10.x, newTriangle10.y, newTriangle10.z,
        newTriangle11.x, newTriangle11.y, newTriangle11.z,
        newTriangle12.x, newTriangle12.y, newTriangle12.z,
        newTriangle13.x, newTriangle13.y, newTriangle13.z,
        newTriangle14.x, newTriangle14.y, newTriangle14.z,
        newTriangle15.x, newTriangle15.y, newTriangle15.z,
        newTriangle16.x, newTriangle16.y, newTriangle16.z,
        newTriangle17.x, newTriangle17.y, newTriangle17.z,
        newTriangle18.x, newTriangle18.y, newTriangle18.z,
        newTriangle19.x, newTriangle19.y, newTriangle19.z,
        newTriangle20.x, newTriangle20.y, newTriangle20.z,
        newTriangle21.x, newTriangle21.y, newTriangle21.z,
        newTriangle22.x, newTriangle22.y, newTriangle22.z,
        newTriangle23.x, newTriangle23.y, newTriangle23.z
    };

    float vertices2[] = {
        // 0.5f,  0.5f, 0.0f,  // top right
        // 0.5f, -0.5f, 0.0f,  // bottom right
        //-0.5f, -0.5f, 0.0f,  // bottom left
        //-0.5f,  0.5f, 0.0f   // top left 
        someTriangle0.x, someTriangle0.y, someTriangle0.z,
        someTriangle1.x, someTriangle1.y, someTriangle1.z,
        someTriangle2.x, someTriangle2.y, someTriangle2.z,
        someTriangle3.x, someTriangle3.y, someTriangle3.z,
        someTriangle4.x, someTriangle4.y, someTriangle4.z,
        someTriangle5.x, someTriangle5.y, someTriangle5.z,
        someTriangle6.x, someTriangle6.y, someTriangle6.z,
        someTriangle7.x, someTriangle7.y, someTriangle7.z,
        someTriangle8.x, someTriangle8.y, someTriangle8.z,
        someTriangle9.x, someTriangle9.y, someTriangle9.z,
        someTriangle10.x, someTriangle10.y, someTriangle10.z,
        someTriangle11.x, someTriangle11.y, someTriangle11.z,
        someTriangle12.x, someTriangle12.y, someTriangle12.z,
        someTriangle13.x, someTriangle13.y, someTriangle13.z,
        someTriangle14.x, someTriangle14.y, someTriangle14.z,
        someTriangle15.x, someTriangle15.y, someTriangle15.z,
        someTriangle16.x, someTriangle16.y, someTriangle16.z,
        someTriangle17.x, someTriangle17.y, someTriangle17.z,
        someTriangle18.x, someTriangle18.y, someTriangle18.z,
        someTriangle19.x, someTriangle19.y, someTriangle19.z,
        someTriangle20.x, someTriangle20.y, someTriangle20.z,
        someTriangle21.x, someTriangle21.y, someTriangle21.z,
        someTriangle22.x, someTriangle22.y, someTriangle22.z,
        someTriangle23.x, someTriangle23.y, someTriangle23.z
    };

    unsigned int indices[] = {  // note that we start from 0!

        //0, 6, 2, // back face
        //0, 4, 6, //
        0, 2, 3, // right face
        1, 0, 3, //
        //5, 1, 7, // front face
        //1, 3, 7, //
        4, 5, 6, // left face 
        6, 5, 7, //
        3, 7, 6, // down face
        3, 6, 2, //
        4, 0, 1, // top face
        4, 1, 5,
        // first Triangle
        //1, 2, 3   // second Triangle

        // Second box
        8, 10, 11, // right face
        9, 8, 11, //
        12, 13, 14, // left face 
        14, 13, 15, //
        11, 15, 14, // down face
        11, 14, 10, //
        12, 8, 9, // top face
        12, 9, 13,

        16, 18, 19, // right face
        17, 16, 19, //
        20, 21, 22, // left face 
        22, 21, 23, //
        19, 23, 22, // down face
        19, 22, 18, //
        20, 16, 17, // top face
        20, 17, 21
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), &vertices2, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    bool increasing = true, movingAway = true;
    float num = 0;
    float speed = 0.1;

    int project_trans_mat1, project_trans_mat2, project_trans_mat3, project_trans_mat4, colourLocation, proj_trans_mat;

    while (!glfwWindowShouldClose(window)) {
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), &vertices2, GL_DYNAMIC_DRAW);
        
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw our first triangle
        proj_trans_mat = glGetUniformLocation(shaderProgram, "project_trans_matrix");
        colourLocation = glGetUniformLocation(shaderProgram, "u_colour");
        //project_trans_mat1 = glGetUniformLocation(shaderProgram, "project_trans_matrix1");
        //project_trans_mat2 = glGetUniformLocation(shaderProgram, "project_trans_matrix2");
        //project_trans_mat3 = glGetUniformLocation(shaderProgram, "project_trans_matrix3");
        //project_trans_mat4 = glGetUniformLocation(shaderProgram, "project_trans_matrix4");
        glUseProgram(shaderProgram);
        glUniform4f(colourLocation, sin(num), sin(num/9), sin(num/3), 1.0f);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glUniformMatrix4fv(proj_trans_mat, 1, GL_FALSE, &projectionMat.data[0][0]);
        glDrawElements(GL_TRIANGLES, 3*36, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(0); // no need to unbind it every time
        if (num >= 3.14*9) {
            increasing = false;
        }
        else if (num <= 0) {
            increasing = true;
        }
        if (increasing) {
            num += speed;
        } else {
            num -= speed;
        }

        vertices2[2] += 10;

        //glUniform3f(distMoved, -100000, -1000, -1000);
        //glUniform4f(project_trans_mat1, projectionMat.data[0][0], -1000, -1000, );
        if (z>maxSightDistance) {
            movingAway = false;
        }
        else if (z < -minSightDistance) {
            movingAway = true;
        }
        if (movingAway){
            z += 10;
        }
        else {
            z -= 10;
        }

        //projectionMat.data[3][3] = z;
        //projectionMat.data[3][2] = projectionMat.data[3][3] * (maxSightDistance / (maxSightDistance - minSightDistance)) - ((float)minSightDistance * maxSightDistance) / (maxSightDistance - minSightDistance);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();        
    }
}

int Graphics::cleanup() {
    return 0;
};

void Graphics::mutliplyMatVec(Vec3D& input, Vec3D& output, mat4x4& someMatrix) {
    output.x = input.x * someMatrix.data[0][0] + input.y * someMatrix.data[1][0] + input.z * someMatrix.data[2][0] + someMatrix.data[3][0];
    output.y = input.x * someMatrix.data[0][1] + input.y * someMatrix.data[1][1] + input.z * someMatrix.data[2][1] + someMatrix.data[3][1];
    output.z = input.x * someMatrix.data[0][2] + input.y * someMatrix.data[1][2] + input.z * someMatrix.data[2][2] + someMatrix.data[3][2];
    float w = input.x * someMatrix.data[0][3] + input.y * someMatrix.data[1][3] + input.z * someMatrix.data[2][3] + someMatrix.data[3][3];
    if (w != 0) {
        output.x /= w;
        output.y /= w;
        output.z /= w;
    }
}
