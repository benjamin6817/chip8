#pragma once

#include <unordered_map>
#include "Key.h"

enum class KeypadKey
{
    Zero = 0x0,
    One = 0x1,
    Two = 0x2,
    Three = 0x3,
    Four = 0x4,
    Five = 0x5,
    Six = 0x6,
    Seven = 0x7,
    Eight = 0x8,
    Nine = 0x9,
    A = 0xA,
    B = 0xB,
    C = 0xC,
    D = 0xD,
    E = 0xE,
    F = 0xF,
};

typedef std::unordered_map<Key, KeypadKey> KeypadMap;

class Keypad
{
public:
    Keypad();

    void reset();
    void updateKey(Key key, bool isPressed);
    bool isKeyPressed(int key) const;
    int getKey() const;

private:
    static const int KEY_COUNT = 16;
    static const KeypadMap DEFAULT_MAP;
    bool keys[KEY_COUNT];
    KeypadMap map = DEFAULT_MAP;
};
