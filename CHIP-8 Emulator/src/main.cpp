#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Chip8.h"

const char* programName = "Breakout (Brix hack) [David Winter, 1997].ch8";

static void handleKey(Window* window, Key key, bool isKeyPressed)
{
    Chip8* chip8 = static_cast<Chip8*>(window->getUserPointer());
    if (key == Key::Escape && isKeyPressed)
    {
        chip8->reset();
        chip8->loadProgram(programName);
    }
    else
    {
        chip8->updateKeypad(key, isKeyPressed);
    }
}

int main()
{
    Window window(640, 320, "CHIP-8 Emulator");
    window.setKeyHandler(handleKey);

    Chip8 chip8(programName);
    window.setUserPointer(&chip8);

    while (window.isOpen())
    {
        chip8.update();
        window.update();
    }

    return 0;
}
