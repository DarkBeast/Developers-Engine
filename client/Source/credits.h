/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_CREDITS_H
#define DE_CREDITS_H
#include "widget.h"
typedef struct credits_t credits_t;

struct credits_t{
	widget btnclose;
	widget btnback;
	widget lblback;
	widget imgcredits;
	widget frmmain;
	widget wndcredits;
};

void credits(void);
void init_credits(void);
void credits_btnclose_press(widget *control, int button, int pressed);
void credits_btnback_press(widget *control, int button, int pressed);
#endif