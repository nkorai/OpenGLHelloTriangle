#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string readFileIntoString(const string& path) {
    ostringstream sstream;
    ifstream fs(path);
    sstream << fs.rdbuf();
    const string str(sstream.str());
    return str;
}

int main() {
    // Start GL context and OS window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "failed to init GLEW");
        return 1;
    }

    // Get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // Get version string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported: %s\n", version);

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth testing interprets a smaller value as "closer"
    glEnable(GL_PROGRAM_POINT_SIZE);

    /* End setup, start custom drawing code */
    // Format: xyzxyzxyz
    float points1[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    GLuint vbo1 = 0;
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points1, GL_STATIC_DRAW);

    GLuint vao1 = 0;
    glGenVertexArrays(1, &vao1);
    glBindVertexArray(vao1);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    float points2[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    GLuint vbo2 = 0;
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points2, GL_STATIC_DRAW);

    GLuint vao2 = 0;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Vertex shader
    string vertShaderString = readFileIntoString("./shaders/test_vs.glsl");
    const char* vertex_shader = vertShaderString.c_str();

    // Fragment shader yellow
    string fragmentShaderYellowString = readFileIntoString("./shaders/test_fs_yellow.glsl");
    const char* fragment_shader_yellow = fragmentShaderYellowString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    GLuint fs_yellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_yellow, 1, &fragment_shader_yellow, NULL);
    glCompileShader(fs_yellow);

    GLuint shader_program_yellow = glCreateProgram();
    glAttachShader(shader_program_yellow, fs_yellow);
    glAttachShader(shader_program_yellow, vs);
    glLinkProgram(shader_program_yellow);

    // Fragment shader pink
    string fragmentShaderPinkString = readFileIntoString("./shaders/test_fs_pink.glsl");
    const char* fragment_shader_pink = fragmentShaderPinkString.c_str();

    GLuint fs_pink = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs_pink, 1, &fragment_shader_pink, NULL);
    glCompileShader(fs_pink);

    GLuint shader_program_pink = glCreateProgram();
    glAttachShader(shader_program_pink, fs_pink);
    glAttachShader(shader_program_pink, vs);
    glLinkProgram(shader_program_pink);

    glClearColor(0.0f, 0.9f, 0.9f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        // Wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// First
        glUseProgram(shader_program_yellow);
        glBindVertexArray(vao1);

        // Draw points 0-3 from the currently bound VAO with the current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //// Second
        glUseProgram(shader_program_pink);
        glBindVertexArray(vao2);

        // Draw points 0-3 from the currently bound VAO with the current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        // Update other events like input handling
        glfwPollEvents();

        // Send drawings to the display
        glfwSwapBuffers(window);
    }

    // Close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}