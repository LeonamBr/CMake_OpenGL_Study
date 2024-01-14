#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>

const GLuint width = 800;
const GLuint height = 600;

GLuint vertexArrayObject, vertexBufferObject, program;

static const GLchar* vertexShader = R"(
    #version 330

    layout (location = 0) in vec3 a_Position;

    void main()
    {
        gl_Position = vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
    }

)";

static const GLchar* fragmentShader = R"(
    #version 330

    out vec4 a_Color;

    void main()
    {
        a_Color = vec4(0.8f, 0.3f, 0.8f, 1.0f);
    }

)";

void createTriangle();
void compileShader();
void addShader(GLuint program, const GLchar* code, GLenum type);

int main(int argc, char** argv)
{

    if(!glfwInit())
    {
        std::cout << "Couldn't initialize GLFW!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << glfwGetVersionString() << std::endl;

    GLFWwindow* window = glfwCreateWindow(width, height, "Hello world window", nullptr, nullptr);

    if(!window)
    {
        std::cout << "Failed to create glfw window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load glad modern openGL loader" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    createTriangle();
    compileShader();

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vertexArrayObject);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void createTriangle()
{

    GLfloat vertices[] = {
        -0.5, -0.5, 0.0f, 
         0.0,  0.5, 0.0f, 
         0.5, -0.5, 0.0f 
    };

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void compileShader()
{

    program = glCreateProgram();

    if(!program)
    {
        std::cout << "error: can't create a program" << std::endl;
        return;
    }

    addShader(program, vertexShader, GL_VERTEX_SHADER);
    addShader(program, fragmentShader, GL_FRAGMENT_SHADER);
    GLint result;

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if(!result)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        GLchar* message = (GLchar*)alloca(length * sizeof(GLchar*));
        glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, &length, message);
        std::cout << "Error linking program!" << std::endl;
        std::cout << message << std::endl;
        return;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &result);

    if(!result)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        GLchar* message = (GLchar*)alloca(length * sizeof(GLchar*));
        glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, &length, message);
        std::cout << "Error validating program!" << std::endl;
        std::cout << message << std::endl;
        return;
    }

}

void addShader(GLuint program, const GLchar *code, GLenum type)
{

    GLuint shader = glCreateShader(type);

    if(!shader)
    {
        std::cout << "Couldn't create shader program" << std::endl;
        return;
    }

    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint result;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if(!result)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        GLchar* message = (GLchar*)alloca(length * sizeof(GLchar*));
        glGetProgramInfoLog(shader, GL_INFO_LOG_LENGTH, &length, message);
        std::cout << "Error compiling shader program!" << std::endl;
        std::cout << message << std::endl;
        return;
    }

    glAttachShader(program, shader);


}
