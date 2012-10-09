#include "Keys.h"
#include <stdlib.h>

void GLFWCALL handleKeypress(int key,int press) //The key that was pressed
{
	switch (key) {
		case GLFW_KEY_ESC: //Escape key
			exit(0); //Exit the program
	}
}

