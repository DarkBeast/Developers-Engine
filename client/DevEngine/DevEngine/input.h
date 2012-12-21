/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef _INPUT_H
#define _INPUT_H

void GLFWCALL handlekeypress(int key,int press);
void GLFWCALL handlemousepos(int x,int y);
void GLFWCALL handlemousepress(int button,int pressed);
void GLFWCALL handlemousewheel(int moved);
#endif