/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*
******************************************************************************/
#include "globals.h"

/*examples of a global variable*/
int playersonline;

void setplayersonline(int players){ playersonline = players; }

void increaseplayersonline(void){ playersonline += 1; }

void decreaseplayersonline(void){ playersonline -= 1; }

int getplayersonline(void){ return playersonline; }