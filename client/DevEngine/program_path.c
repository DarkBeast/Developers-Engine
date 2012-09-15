

#include "program_path.h"
#include <stdlib.h>
#include <string.h>
#include <direct.h>

char* getpath(char* target)
{
	strcpy (Path ,Program_Path);
	strcat(Path ,target);

	return Path;
}

int CDL_GetCurrentDirectory(Uint32 iTarget, Uint32 *iRequired)

{

  char  *sPath   = NULL;
  char  *sResult = NULL;
  Uint32 iPath   = 0;
  Uint32 iSize   = 1024;

  
  Program_Path = (char *)malloc(iTarget + 1);
  sPath = (char *)malloc(iSize);
  Path = (char *)malloc(iTarget + 1);
  
  if (Program_Path == NULL)
	{
		return -1;
	}

  if (sPath == NULL)

  {

    return -1;

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


  if (Program_Path != NULL && iTarget >= iPath)

  {

    memcpy(Program_Path, sPath, iPath);

  }


  if (iRequired != NULL)

  {

    *iRequired = iPath;

  }

  free(sPath);

  if (Program_Path == NULL || iTarget < iPath)

  {

    return -1;

  }

  return 0;

}

void pathdestroy(void)
{
	Path =(char *) malloc (sizeof(Path+1024));
	Program_Path =(char *) malloc (sizeof(Program_Path+1024));

	free(Program_Path);
	
	free(Path);
	return;
}
