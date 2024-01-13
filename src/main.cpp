#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

GLuint Width  = 800;
GLuint Height = 600;

int main(int argc, char** argv)
{

    if(!glfwInit())
    {
        std::cout << "Couldn't load glfw!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    //MacOs System
    //glfwWindowHint(GLFW_OPENGL_FOWARD_COMPAT, GL_TRUE);

    std::cout << glfwGetVersionString() << std::endl;

    GLFWwindow* window = glfwCreateWindow(Width, Height, "Hello World Window", nullptr, nullptr);

    if(!window)
    {
        std::cout << "couldn't Create glfw window!" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load glad!!" << std::endl;
        return -1;
    }

    glfwSwapInterval(1);

    GLfloat xDirection = 0.0f;
    GLfloat increment = 0.01f;

    while(!glfwWindowShouldClose(window))
    {

        glClearColor(0.8f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

        glVertex3f( -0.5f + xDirection, -0.5f, 0.0f);
        glVertex3f(  0.0f + xDirection,  0.5f, 0.0f);
        glVertex3f(  0.5f + xDirection, -0.5f, 0.0f);

        glEnd();

        if(xDirection > 0.5f)
            increment = (-increment);
        if(xDirection < -0.5)
            increment = (-increment);

        xDirection += increment;

        glfwSwapBuffers(window);

        glfwPollEvents();

    }

    glfwTerminate();
    
    return 0;
}