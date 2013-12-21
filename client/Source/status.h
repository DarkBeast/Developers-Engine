/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_STATUS_H
#define DE_STATUS_H
#include "widget.h"
typedef struct status_t status_t;

struct status_t{
	widget btnclose;
	widget btnok;
	widget lblok;
	widget lbltext;
	widget frmmain;
	widget wndhide;
	widget wndstatus;
};

void set_status(char *text);

void status_unload(void);

int status(void *arg);

void draw_status(widget *control);

void init_status_window(sbool accept, double counter);

void init_status_widget(widget *parent, sbool accept, double counter, sbool disable_controls);

void status_btnclose_press(widget *control, int button, int pressed);

void status_btnok_press(widget *control, int button, int pressed);
#endif