

#include "program_path.h"
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "globals.h"
#include"integer.h"

char* GetPath(char* target)
{
	strcpy (Path ,Program_Path);
	strcat(Path ,target);

	return Path;
}

int CDLGetCurrentDirectory() 
{ //Uint32 iTarget, Uint32 *iRequired

  char  *sPath   = NULL;
  char  *sResult = NULL;
  Uint32 iPath   = 0;
  Uint32 iSize   = 1024;

  Program_Path = (char *)malloc(iSize  + 1);
  sPath = (char *)malloc(iSize);
  Path = (char *)malloc(iSize  + 1);
  
  if (Program_Path == NULL)
	{
		return false;
	}

  if (sPath == NULL)

  {

    return false;

  }


  sResult = getcwd(sPath, iSize);

  while (sResult == NULL)

  {

    iSize *= 2;

    
    sPath = (char *)realloc(sPath, iSize);

    
    if (sPath == NULL)

    {

      return -1;

    }

    
    sResult = getcwd(sPath, iSize);

  }

 
  iPath = strlen(sPath) + 1;


  if (Program_Path != NULL && iSize  >= iPath)

  {

    memcpy(Program_Path, sPath, iPath);

  }


  free(sPath);
  strcat(Program_Path ,"\\");
  if (Program_Path == NULL || iSize  < iPath)
  {

    return false;

  }

  

  return true;

}

void pathdestroy(void)
{
	Path =(char *) malloc (sizeof(Path+1024));
	Program_Path =(char *) malloc (sizeof(Program_Path+1024));

	free(Program_Path);
	
	free(Path);
	return;
}
