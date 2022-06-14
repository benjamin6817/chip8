#include "Chip8.h"
#include <iostream>
#include <cstdlib>

const std::uint8_t Chip8::FONTSET[FONTSET_SIZE] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
    init();
}

Chip8::Chip8(const char* programPath)
{
    init();
    loadProgram(programPath);
}

void Chip8::init()
{
    reset();
    display.init();
}

void Chip8::reset()
{
    // Clear registers
    I = 0;
    for (int i = 0; i < REGISTER_COUNT; ++i)
    {
        V[i] = 0;
    }

    // Clear memory
    for (int i = 0; i < MEMORY_SIZE; ++i)
    {
        memory[i] = 0;
    }

    // Clear display
    display.clear();

    // Clear keypad state
    keypad.reset();

    // Load fontset
    for (int i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[i] = FONTSET[i];
    }

    // Seed random number generator
    srand(time(NULL));

    // Clear screen
    shouldRedraw = true;
}

void Chip8::loadProgram(const char* path)
{
    FILE* program;
    fopen_s(&program, path, "rb");
    if (program == NULL)
    {
        std::cerr << "Unable to open program: " << path << ".\n";
        return;
    }
    fread(&memory[PROGRAM_START], 1, MAX_PROGRAM_SIZE, program);
    fclose(program);
    pc = PROGRAM_START;
}

void Chip8::incrementPC()
{
    pc += 2;
}

void Chip8::drawSprite(std::uint8_t Vx, std::uint8_t Vy, std::uint8_t n)
{
    uint8_t pixel;
    V[0xF] = 0;
    for (int yline = 0; yline < n; yline++)
    {
        pixel = memory[I + yline];
        for (int xline = 0; xline < 8; xline++)
        {
            if (0 != (pixel & (0x80 >> xline)))
            {
                int x = Vx + xline;
                int y = Vy + yline;
                if (display.get(x, y) == Pixel::White)
                {
                    V[0xF] = 1;
                    display.set(x, y, Pixel::Black);
                }
                else
                {
                    display.set(x, y, Pixel::White);
                }
            }
        }
    }
    shouldRedraw = true;
}

void Chip8::executeOpcode()
{
    std::uint16_t opcode = memory[pc] << 8 | memory[pc + 1];
    std::uint8_t x = opcode >> 8 & 0x000F;
    std::uint8_t y = opcode >> 4 & 0x000F;
    std::uint8_t n = opcode & 0x000F;
    std::uint8_t kk = opcode & 0x00FF;
    std::uint16_t nnn = opcode & 0x0FFF;

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode) {
        case 0x00E0:
            display.clear();
            incrementPC();
            break;
        case 0x00EE:
            pc = callStack.top();
            callStack.pop();
            incrementPC();
            break;
        default:
            break;
        }
        break;
    case 0x1000:
        pc = nnn;
        break;
    case 0x2000:
        callStack.push(pc);
        pc = nnn;
        break;
    case 0x3000:
        if (V[x] == kk)
        {
            incrementPC();
        }
        incrementPC();
        break;
    case 0x4000:
        if (V[x] != kk)
        {
            incrementPC();
        }
        incrementPC();
        break;
    case 0x5000:
        if (V[x] == V[y])
        {
            incrementPC();
        }
        incrementPC();
        break;
    case 0x6000:
        V[x] = kk;
        incrementPC();
        break;
    case 0x7000:
        V[x] += kk;
        incrementPC();
        break;
    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000:
            V[x] = V[y];
            incrementPC();
            break;
        case 0x0001:
            V[x] |= V[y];
            incrementPC();
            break;
        case 0x0002:
            V[x] &= V[y];
            incrementPC();
            break;
        case 0x0003:
            V[x] ^= V[y];
            incrementPC();
            break;
        case 0x0004:
            if (V[y] > (0xFF - V[x]))
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] += V[y];
            incrementPC();
            break;
        case 0x0005:
            if (V[y] > V[x])
            {
                V[0xF] = 0;
            }
            else
            {
                V[0xF] = 1;
            }
            V[x] -= V[y];
            incrementPC();
            break;
        case 0x0006:
            V[0xF] = V[x] & 0x1;
            V[x] >>= 1;
            incrementPC();
            break;
        case 0x0007:
            if (V[x] > V[y])
            {
                V[0xF] = 0;
            }
            else
            {
                V[0xF] = 1;
            }
            V[x] = V[y] - V[x];
            incrementPC();
            break;
        case 0x000E:
            V[0xF] = V[x] >> 7;
            V[x] <<= 1;
            incrementPC();
            break;
        default:
            break;
        }
        break;
    case 0x9000:
        if (V[x] != V[y])
        {
            incrementPC();
        }
        incrementPC();
        break;
    case 0xA000:
        I = nnn;
        incrementPC();
        break;
    case 0xB000:
        pc = nnn + V[0];
        break;
    case 0xC000:
        V[x] = (rand() % 0xFF) & kk;
        incrementPC();
        break;
    case 0xD000:
        drawSprite(V[x], V[y], n);
        incrementPC();
        break;
    case 0xE000:
        switch (opcode & 0x00FF) {
        case 0x009E:
            if (keypad.isKeyPressed(V[x]))
            {
                incrementPC();
            }
            incrementPC();
            break;
        case 0x00A1:
            if (!keypad.isKeyPressed(V[x]))
            {
                incrementPC();
            }
            incrementPC();
            break;
        default:
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF)
        {
        case 0x0007:
            V[x] = delayTimer;
            incrementPC();
            break;
        case 0x000A:
        {
            int key = keypad.getKey();
            if (key == -1)
            {
                return;
            }
            V[x] = key;
            incrementPC();
            break;
        }
        case 0x0015:
            delayTimer = V[x];
            incrementPC();
            break;
        case 0x0018:
            soundTimer = V[x];
            incrementPC();
            break;
        case 0x001E:
            if (I + V[x] > 0xFFF)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            I += V[x];
            incrementPC();
            break;
        case 0x0029:
            I = V[x] * 0x5;
            incrementPC();
            break;
        case 0x0033:
            memory[I] = V[x] / 100;
            memory[I + 1] = (V[x] / 10) % 10;
            memory[I + 2] = (V[x] % 100) % 10;
            incrementPC();
            break;
        case 0x0055:
            for (int i = 0; i <= x; ++i)
            {
                memory[I + i] = V[i];
            }
            I += (x + 1);
            incrementPC();
            break;
        case 0x0065:
            for (int i = 0; i <= x; ++i)
            {
                V[i] = memory[I + i];
            }
            I += (x + 1);
            incrementPC();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void Chip8::update()
{
    executeOpcode();
    updateTimers();

    if (shouldRedraw)
    {
        display.update();
        shouldRedraw = false;
    }
    display.render();
}

void Chip8::updateTimers()
{
    if (delayTimer > 0)
    {
        --delayTimer;
    }

    if (soundTimer > 0)
    {
        if (soundTimer == 1)
        {
            std::cout << "beep\n";
        }
        --soundTimer;
    }
}

void Chip8::updateKeypad(Key key, bool isPressed)
{
    keypad.updateKey(key, isPressed);
}
