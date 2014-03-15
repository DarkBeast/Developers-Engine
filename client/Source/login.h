/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_LOGIN_H
#define DE_LOGIN_H
#include "widget.h"
typedef struct login_t login_t;

struct login_t{
	widget btnclose;
	widget btnback;
	widget btnlogin;
	widget lblback;
	widget lbllogin;
	widget lbllogintxt;
	widget lblpasstxt;
	widget txtname;
	widget txtpass;
	widget frmmain;
	widget wndlogin;
	widget status;
};

void login(void);
void init_login(void);
void login_btnclose_press(widget *control, int button, int pressed);
void login_btnback_press(widget *control, int button, int pressed);
void login_status_button_press(widget *control, int button, int pressed);
void login_status_message(char *text);
#endif