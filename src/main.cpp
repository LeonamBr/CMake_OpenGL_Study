#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

GLuint Width  = 800;
GLuint Height = 600;

GLuint vertexArrayObject, vertexBufferObject, indexBufferObject, program;

GLint colorLocation, transformLocation;

GLfloat xMove = 0.0f, yMove = 0.0f;

const GLchar* vertexShader = R"(
    #version 330

    layout (location = 0) in vec3 a_Position;
    uniform mat4 u_Transform;

    void main()
    {
        gl_Position = u_Transform * vec4(a_Position, 1.0f);
    }
)";

const GLchar* fragmentShader = R"(
    #version 330

    layout (location = 0) out vec4 a_Color;

    uniform vec4 u_Color;

    void main()
    {
        a_Color = u_Color;
    }
)";

void createTriangle();
void compileShader();
void addShader(GLuint program, const GLchar* code, GLenum type);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void move(GLFWwindow* window);

int main(int argc, char** argv)
{

    if(!glfwInit())
    {
        std::cout << "Couldn't initialize GLFW!!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    std::cout << glfwGetVersionString() << std::endl;

    GLFWwindow* window = glfwCreateWindow( Width, Height, "Hello world window", nullptr, nullptr);

    if(!window)
    {
        std::cout << "Failed to create a glfw window!!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Can't load GLAD!!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    createTriangle();
    compileShader();

    glfwSwapInterval(1);

    while(!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        move(window);

        glUseProgram(program);
        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

        glUniform4f(colorLocation, (rand() % 10) / 10.0f, (rand() % 10) / 10.0f, (rand() % 10) / 10.0f, 1.0f);
        glm::mat4 model{1.0f};
        model = glm::translate(model, glm::vec3(xMove, yMove, 0.0f));

        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;

}

void createTriangle()
{

    GLfloat vertices[] = {
        -0.1f, -0.1f, 0.0f,
        -0.1f,  0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
         0.1f,  0.1f, 0.0f

    };

    GLint indices[] {
        0, 1, 2,
        2, 3, 1
    };


    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void compileShader()
{

    program =  glCreateProgram();

    if(!program)
    {
        std::cout << "Couldn't create shader program" << std::endl;
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
        GLchar* message =  (GLchar*)alloca(length * sizeof(GLchar*));
        glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, &length, message);
        std::cout << "Failed to link shader program" << std::endl;
        std::cout << message << std::endl;
        return;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if(!result)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        GLchar* message =  (GLchar*)alloca(length * sizeof(GLchar*));
        glGetProgramInfoLog(program, GL_INFO_LOG_LENGTH, &length, message);
        std::cout << "Failed to validate shader program" << std::endl;
        std::cout << message << std::endl;
        return;
    }

    colorLocation = glGetUniformLocation(program, "u_Color");
    transformLocation = glGetUniformLocation(program, "u_Transform");

}

void addShader(GLuint program, const GLchar *code, GLenum type)
{

    GLuint shader = glCreateShader(type);

    if(!shader)
    {
        std::cout << "Can't create shader" << std::endl;
        return;
    }

    glShaderSource(shader, 1, &code, nullptr);

    GLint result;

    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if(!result)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        GLchar* message =  (GLchar*)alloca(length * sizeof(GLchar*));
        glGetShaderInfoLog(shader, GL_INFO_LOG_LENGTH, &length, message);
        std::cout << "Failed to compile shader code" << std::endl;
        std::cout << message << std::endl;
        return;
    }

    glAttachShader(program, shader);

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);
}

void move(GLFWwindow* window)
{

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        xMove += 0.01f;
    } else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        xMove -= 0.01f;
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        yMove += 0.01f;
    } else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        yMove -= 0.01f;
    }

}
