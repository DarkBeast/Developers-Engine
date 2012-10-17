/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis      
******************************************************************************/

#include <glfw.h>
#include <stdlib.h>
#include "Keys.h"
#include "Image.h"
#include "Render.h"

void GLFWCALL handleKeypress(int key,int press) //The key that was pressed
{

	//TODO: make HANDLE ARRAY FOR KEYS THAT ARE PUSHED SO IF MULTIPLE KEYS ARE PRESSED IT CAN HANDLE THEM ALL.
	switch (key) {
	case GLFW_KEY_ESC: //Escape key
		exit(0); //Exit the program
	case GLFW_KEY_UP: 
		if (pos.y > 0){

			pos.y = pos.y -1;
		}
		break;
	case GLFW_KEY_DOWN: 
		if (pos.y < screen.Height){
			pos.y = pos.y +1;
		}
		break;
	case GLFW_KEY_LEFT: 
		if (pos.x >0){
			pos.x = pos.x -1;
		}
		break;
	case GLFW_KEY_RIGHT: 
		if (pos.x < screen.Width){
			pos.x = pos.x +1;
		}
		break;
	}
}

