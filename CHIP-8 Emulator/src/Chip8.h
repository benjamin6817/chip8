#pragma once

#include <cstddef>
#include <cstdint>
#include <stack>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include "Display.h"
#include "Keypad.h"

class Chip8
{
public:
    Chip8();
    Chip8(const char* programPath);

    void init();
    void reset();
    void loadProgram(const char* path);
    void update();
    void updateKeypad(Key key, bool isPressed);

private:
    void executeOpcode();
    void incrementPC();
    void drawSprite(std::uint8_t x, std::uint8_t y, std::uint8_t n);
    void updateTimers();

    static const int REGISTER_COUNT = 16;
    static const int PROGRAM_START = 512;
    static const int MEMORY_SIZE = 4096;
    static const int MAX_PROGRAM_SIZE = MEMORY_SIZE - PROGRAM_START;
    static const int SPRITE_WIDTH = 8;
    static const int FONTSET_SIZE = 80;
    static const std::uint8_t FONTSET[FONTSET_SIZE];

    bool shouldRedraw = false;
    Display display;
    Keypad keypad;
    std::uint8_t memory[MEMORY_SIZE];
    std::stack<std::uint16_t> callStack;
    std::uint8_t V[REGISTER_COUNT];
    std::uint16_t I = 0;
    std::uint16_t pc = PROGRAM_START;
    std::uint8_t delayTimer = 0;
    std::uint8_t soundTimer = 0;
};

