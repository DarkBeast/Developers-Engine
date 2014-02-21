/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_MAINMENU_H
#define DE_MAINMENU_H
#include "widget.h"
typedef struct main_menu_t main_menu_t;

struct main_menu_t{
	widget btnclose;
	widget btnlogin;
	widget btncredits;
	widget btncreate;
	widget lbllogin;
	widget lblcredits;
	widget lblcreate;
	widget frmmain;
	widget wndmainmenu;
	widget testbox;
};

void main_menu(void);
void init_main_menu(void);
void main_btnclose_press(widget *control, int button, int pressed);
void main_btncredits_press(widget *control, int button, int pressed);
void main_btnlogin_press(widget *control, int button, int pressed);
void main_btncreate_press(widget *control, int button, int pressed);
void render_test(widget *control);
void test_buffer(void);

#endif