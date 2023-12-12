#include "KeyboardManager.hpp"

void KeyboardManager::keyHandler(unsigned char key, int, int) // Callback When normal key is pressed down
{
    keyboard(key);
}
void KeyboardManager::specialKeyHandler(int key, int, int) // Callback when special key is pressed down
{
    specialKeyboard(key);
}