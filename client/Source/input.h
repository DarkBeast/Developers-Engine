/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_INPUT_H
#define DE_INPUT_H

#include <glfw3.h>

void handle_char_callback(GLFWwindow *screen, unsigned int key);
void handle_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void handle_mouse_pos(GLFWwindow * screen,double x,double y);
void handle_mouse_press(GLFWwindow * screen, int button, int pressed, int mods);
void handle_mouse_wheel(GLFWwindow * screen, double x, double y);
void handle_window_close(GLFWwindow * screen);
#endif