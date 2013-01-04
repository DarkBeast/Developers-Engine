/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_INPUT_H
#define DE_INPUT_H

void GLFWCALL handlekeypress(int key,int press);
void GLFWCALL handlemousepos(int x,int y);
void GLFWCALL handlemousepress(int button,int pressed);
void GLFWCALL handlemousewheel(int moved);
#endif