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
    // check for shader compile errorss
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
    
    float x = 0, y = 0, z = 0;

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
    Vec3D someTriangle2 = Vec3D(1000.0f, 0, 1000.0f);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------ 


    float vertices2[] = {
         500,  500, 1500,  // top right
         500, -500, 1500,  // bottom right
        -500, -500, 1500,  // bottom left
        -500,  500, 1500,   // top left 
         500,  1000, 1500,
        //someTriangle0.x, someTriangle0.y, someTriangle0.z,
        //someTriangle1.x, someTriangle1.y, someTriangle1.z,
        //someTriangle2.x, someTriangle2.y, someTriangle2.z
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 2,
        2, 3, 0,
        0, 4, 1
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_DYNAMIC_DRAW);

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

    int coolDown = 0;

    int colourLocation, proj_trans_mat, state;

    double cx, cy, nx, ny;
    glfwGetCursorPos(window, &cx, &cy);

    while (!glfwWindowShouldClose(window)) {
        glfwGetCursorPos(window, &nx, &ny);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), &vertices2, GL_DYNAMIC_DRAW);
        
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        proj_trans_mat = glGetUniformLocation(shaderProgram, "project_trans_matrix");
        colourLocation = glGetUniformLocation(shaderProgram, "u_colour");
        
        glUseProgram(shaderProgram);
        glUniform4f(colourLocation, sin(num), sin(num/9), sin(num/3), 1.0f);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glUniformMatrix4fv(proj_trans_mat, 1, GL_FALSE, &projectionMat.data[0][0]);
        glDrawElements(GL_TRIANGLES, 3*3, GL_UNSIGNED_INT, 0);

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

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        state = glfwGetKey(window, GLFW_KEY_W);
        if (state == GLFW_PRESS)
            z += -100;
        state = glfwGetKey(window, GLFW_KEY_A);
        if (state == GLFW_PRESS)
            x -= 100;
        state = glfwGetKey(window, GLFW_KEY_S);
        if (state == GLFW_PRESS)
            z -= -100;
        state = glfwGetKey(window, GLFW_KEY_D);
        if (state == GLFW_PRESS)
            x += 100;
        state = glfwGetKey(window, GLFW_KEY_SPACE);
        if (state == GLFW_PRESS)
            y += 100;
        state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
        if (state == GLFW_PRESS)
            y -= 100;

        if (nx != cx) {
            for (int index = 0; index < 5; index++) {
                float oldAngle = atan((vertices2[index * 3]) / (vertices2[index * 3 + 2]));
                float newAngle = oldAngle + ((nx - cx) / screenWidth) * 2 * pi * 3;
                float oldX = vertices2[index * 3], oldZ = vertices2[index * 3 + 2];
                vertices2[index * 3] = sin(newAngle)*oldX/sin(oldAngle);
                vertices2[index * 3 + 2] = cos(newAngle) * oldZ / cos(oldAngle);
            }
        }

        projectionMat.data[3][0] = x * aspectRatio * fieldOfView;
        projectionMat.data[3][1] = y * fieldOfView;
        projectionMat.data[3][2] = z * (maxSightDistance / (maxSightDistance - minSightDistance)) - ((float)minSightDistance * maxSightDistance) / (maxSightDistance - minSightDistance);
        projectionMat.data[3][3] = z;
        cx = nx;
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
