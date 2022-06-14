#pragma once

#include <cstdint>
#include <GL/glew.h>

enum class Pixel
{
    Black = 0,
    White = 1,
};

class Display
{
public:
    Display();
    ~Display();

    void init();
    void update();
    void render();
    void clear();
    void set(int x, int y, Pixel p);
    Pixel get(int x, int y) const;

    static const int WIDTH = 64;
    static const int HEIGHT = 32;
private:
    void createBuffers();
    void createShaders();
    void createTexture();
    void cleanUp();

    static const int SIZE = WIDTH * HEIGHT;
    static const int NUM_COLORS = 3;
    static const char* vertexSource;
    static const char* fragmentSource;

    std::uint8_t bitmap[HEIGHT][WIDTH][NUM_COLORS];

    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint texture;
};

