/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_INPUT_H
#define DE_INPUT_H

void GLFWCALL handle_key_press(int key,int press);
void GLFWCALL handle_mouse_pos(int x,int y);
void GLFWCALL handle_mouse_press(int button,int pressed);
void GLFWCALL handle_mouse_wheel(int moved);
#endif