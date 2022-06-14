#pragma once

#include <GLFW/glfw3.h>
#include "Key.h"

class Window;

typedef void (*KeyHandler)(Window* window, Key key, bool isPressed);
typedef void (*ResizeHandler)(Window* window, int width, int height);

class Window
{
public:
    Window(int initialWidth, int initialHeight, const char* title);
    ~Window();

    void setUserPointer(void* pointer);
    void setKeyHandler(KeyHandler handler);
    void setResizeHandler(ResizeHandler handler);

    void* getUserPointer() const;

    void update();
    bool isOpen() const;
    void close();

    KeyHandler keyHandler;
    ResizeHandler resizeHandler;

private:
    void init(int width, int height, const char* title);
    void destroy();
    void printGlfwVersion() const;

    static void onError(int errorCode, const char* description);
    static void onKeyEvent(GLFWwindow* win, int key, int scancode, int action, int mods);
    static void onResize(GLFWwindow* win, int width, int height);

    static const int MIN_OPENGL_MAJOR_VERSION = 4;
    static const int MIN_OPENGL_MINOR_VERSION = 5;

    GLFWwindow* window;
    void* userPtr;
};
