#include "Error.h"
#include <stdio.h>
#include <stdlib.h>
#include <glfw.h>

//TODO: Add Error File Save

void FatalError(int error_flag)
{
  switch(error_flag)
  {
    case MISC_ERROR:
      fputs("An error has occured!\n", stderr);
      break;
    case POINTER_IN_USE:
      fputs("An error has occured: Pointer in use!\n", stderr);
      break;
    case UNDEFINED_POINTER:
      fputs("An error has occured: Undefined Pointer!\n", stderr);
      break;
    case OUT_OF_MEMORY:
      fputs("An error has occured: Server Out Of Memory!\n", stderr);
      break;
    case IO_ERROR:
      fputs("An error has occured: I/O Error!\n", stderr);
      break;
    case FILE_ERROR:
      fputs("An error has occured: File Error!\n", stderr);
      break;
    case OVERFLOW_ERROR:
      fputs("An error has occured: Overflow!\n", stderr);
      break;
    case SOCKET_ADDR_ERROR:
      fputs("Could not establish server address!\n", stderr);
      break;
    default:
      fputs("An error has occured!\n", stderr);
  }
  fputs("The Application will now close.", stderr);
  
  ShutDown();
}

void RenderError(int error_flag)
{
  switch(error_flag)
  {
    case MISC_ERROR:
      fputs("An error has occured!\n", stderr);
      break;
    case POINTER_IN_USE:
      fputs("An error has occured: Pointer in use!\n", stderr);
      break;
    case UNDEFINED_POINTER:
      fputs("An error has occured: Undefined Pointer!\n", stderr);
      break;
    case OUT_OF_MEMORY:
      fputs("An error has occured: Server Out Of Memory!\n", stderr);
      break;
    case IO_ERROR:
      fputs("An error has occured: I/O Error!\n", stderr);
      break;
    case FILE_ERROR:
      fputs("An error has occured: File Error!\n", stderr);
      break;
    case OVERFLOW_ERROR:
      fputs("An error has occured: Overflow!\n", stderr);
      break;
	 case GLFWINIT_ERROR:
      fputs("An error has occured: GLFW Did Not Intalize!\n", stderr);
      break;
	  case GLFWWIN_ERROR:
      fputs("An error has occured: GLFW Did Not Open Window!\n", stderr);
      break;
    default:
      fputs("An error has occured!\n", stderr);
  }
  fputs("The Application will now close.", stderr);
  
  ShutDown();
}

void ShutDown()
{
	//TODO: Clean up memory, shut down sockets, etc....?
glfwTerminate();
exit( EXIT_FAILURE );
}