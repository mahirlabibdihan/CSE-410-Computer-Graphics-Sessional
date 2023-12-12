#ifndef _KEYBOARD_MANAGER_H
#define _KEYBOARD_MANAGER_H
class KeyboardManager
{
public:
    static void keyHandler(unsigned char, int, int);
    static void specialKeyHandler(int, int, int);
    static void keyboard(unsigned char);
    static void specialKeyboard(int);
};
#endif