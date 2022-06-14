#include <GLFW/glfw3.h>
#include "Keypad.h"

const KeypadMap Keypad::DEFAULT_MAP =
{
    { Key::Alpha1, KeypadKey::One },
    { Key::Alpha2, KeypadKey::Two },
    { Key::Alpha3, KeypadKey::Three },
    { Key::Alpha4, KeypadKey::C },
    { Key::Q, KeypadKey::Four },
    { Key::W, KeypadKey::Five },
    { Key::E, KeypadKey::Six },
    { Key::R, KeypadKey::D },
    { Key::A, KeypadKey::Seven },
    { Key::S, KeypadKey::Eight },
    { Key::D, KeypadKey::Nine },
    { Key::F, KeypadKey::E },
    { Key::Z, KeypadKey::A },
    { Key::X, KeypadKey::Zero },
    { Key::C, KeypadKey::B },
    { Key::V, KeypadKey::F }
};

Keypad::Keypad()
{
    reset();
}

void Keypad::reset()
{
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        keys[i] = false;
    }
}

void Keypad::updateKey(Key key, bool isPressed)
{
    if (map.find(key) != map.end())
    {
        int kc = (int) map[key];
        keys[kc] = isPressed;
    }
}

bool Keypad::isKeyPressed(int key) const
{
    return keys[key];
}

int Keypad::getKey() const
{
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        if (keys[i])
        {
            return i;
        }
    }
    return -1;
}
