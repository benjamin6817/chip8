#include <iostream>
#include <GL/glew.h>
#include "Window.h"

Window::Window(int initialWidth, int initialHeight, const char* title)
{
    init(initialWidth, initialHeight, title);
}

Window::~Window()
{
    destroy();
}

void Window::setUserPointer(void* ptr)
{
    userPtr = ptr;
}

void Window::setKeyHandler(KeyHandler handler)
{
    keyHandler = handler;
}

void Window::setResizeHandler(ResizeHandler handler)
{
    resizeHandler = handler;
}

void* Window::getUserPointer() const
{
    return userPtr;
}

void Window::update()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(window);
}

void Window::close()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::init(int width, int height, const char* title)
{
    glfwSetErrorCallback(onError);

    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed.\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MIN_OPENGL_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MIN_OPENGL_MINOR_VERSION);
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window)
    {
        std::cerr << "GLFW window context creation failed.\n";
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, onKeyEvent);
    glfwSetFramebufferSizeCallback(window, onResize);

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        const GLubyte* errorMessage = glewGetErrorString(glewError);
        std::cerr << "GLEW initialization failed: " << errorMessage << '\n';
        destroy();
        return;
    }

    printGlfwVersion();
}

void Window::destroy()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::printGlfwVersion() const
{
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    std::cout << "GLFW Version " << major << "." << minor << "." << revision << '\n';
}

void Window::onError(int errorCode, const char* description)
{
    const char* error;
    switch (errorCode)
    {
    case GLFW_NOT_INITIALIZED:
        error = "GLFW_NOT_INITIALIZED";
        break;
    case GLFW_NO_CURRENT_CONTEXT:
        error = "GLFW_NO_CURRENT_CONTEXT";
        break;
    case GLFW_INVALID_ENUM:
        error = "GLFW_INVALID_ENUM";
        break;
    case GLFW_INVALID_VALUE:
        error = "GLFW_INVALID_VALUE";
        break;
    case GLFW_OUT_OF_MEMORY:
        error = "GLFW_OUT_OF_MEMORY";
        break;
    case GLFW_API_UNAVAILABLE:
        error = "GLFW_API_UNAVAILABLE";
        break;
    case GLFW_VERSION_UNAVAILABLE:
        error = "GLFW_VERSION_UNAVAILABLE";
        break;
    case GLFW_PLATFORM_ERROR:
        error = "GLFW_PLATFORM_ERROR";
        break;
    case GLFW_FORMAT_UNAVAILABLE:
        error = "GLFW_FORMAT_UNAVAILABLE";
        break;
    case GLFW_NO_WINDOW_CONTEXT:
        error = "GLFW_NO_WINDOW_CONTEXT";
        break;
    default:
        error = "Unknown";
        break;
    }
    std::cerr << error << " Error: " << description << '\n';
}

void Window::onKeyEvent(GLFWwindow* win, int keycode, int scancode, int action, int mods)
{
    void* ptr = glfwGetWindowUserPointer(win);
    Window* window = static_cast<Window*>(ptr);

    if (window->keyHandler == NULL)
    {
        return;
    }

    Key key = static_cast<Key>(keycode);
    if (action == GLFW_PRESS)
    {
        window->keyHandler(window, key, true);
    }
    else if (action == GLFW_RELEASE)
    {
        window->keyHandler(window, key, false);
    }
}

void Window::onResize(GLFWwindow* win, int width, int height)
{
    void* ptr = glfwGetWindowUserPointer(win);
    Window* window = static_cast<Window*>(ptr);

    if (window->resizeHandler == NULL)
    {
        return;
    }

    glViewport(0, 0, width, height);
    window->resizeHandler(window, width, height);
}
