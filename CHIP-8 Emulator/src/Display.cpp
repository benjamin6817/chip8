#include "Display.h"

const char* Display::vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    //in vec3 color;
    in vec2 texcoord;
    //out vec3 Color;
    out vec2 Texcoord;
    void main()
    {
        //Color = color;
        Texcoord = texcoord;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const char* Display::fragmentSource = R"glsl(
    #version 150 core
    //in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D tex;
    void main()
    {
        outColor = texture(tex, Texcoord);
    }
)glsl";

Display::Display()
{
    clear();
}

Display::~Display()
{
    cleanUp();
}

void Display::init()
{
    createBuffers();
    createShaders();
    createTexture();
}

void Display::update()
{
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Display::WIDTH, Display::HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, bitmap);
}

void Display::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Display::clear()
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            for (int z = 0; z < NUM_COLORS; ++z)
            {
                bitmap[y][x][z] = 0;
            }
        }
    }
}

void Display::set(int x, int y, Pixel value)
{
    if (value == Pixel::Black)
    {
        bitmap[y][x][0] = 0;
        bitmap[y][x][1] = 0;
        bitmap[y][x][2] = 0;
    }
    else if (value == Pixel::White)
    {
        bitmap[y][x][0] = 255;
        bitmap[y][x][1] = 255;
        bitmap[y][x][2] = 255;
    }
}

Pixel Display::get(int x, int y) const
{
    if (bitmap[y][x][0] == 255 &&
        bitmap[y][x][1] == 255 &&
        bitmap[y][x][2] == 255)
    {
        return Pixel::White;
    }
    return Pixel::Black;
}

void Display::createBuffers()
{
    // Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create VBO
    glGenBuffers(1, &vbo);
    GLfloat vertices[] = {
        -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &ebo);
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void Display::createShaders()
{
    // Create vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Create shader program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Position attribute
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

    // Color attribute
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // Texture attribute
    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
}

void Display::createTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Display::WIDTH, Display::HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap);
    glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Display::cleanUp()
{
    glDeleteTextures(1, &texture);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
