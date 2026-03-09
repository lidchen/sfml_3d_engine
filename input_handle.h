#ifndef INPUT_HANDLE
#define INPUT_HANDLE

#include "variable.h"
bool isKeyPressedOnce(sf::Keyboard::Key key);
bool isMousePressedOnce(sf::Mouse::Button button);
void input_handle(float dt);
void gaze_handle(float diff_x, float diff_y, float diff_z);

#endif