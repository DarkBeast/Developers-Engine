/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_NEWACCOUNT_H
#define DE_NEWACCOUNT_H
#include "widget.h"
typedef struct new_account_t new_account_t;

struct new_account_t{
	widget btnclose;
	widget frmmain;
	widget wndnewaccount;
	widget btnback;
	widget btncreate;
	widget lblname;
	widget lblusername;
	widget lblpass;
	widget lblrepass;
	widget lblsexm;
	widget lblsexf;
	widget lbljob;
	widget lblback;
	widget lblcreate;
	widget lbljobname;
	widget lblstr;
	widget lbldef;
	widget lblmagic;
	widget lblspeed;
	widget lblstrlbl;
	widget lbldeflbl;
	widget lblmagiclbl;
	widget lblspeedlbl;
	widget txtname;
	widget txtusername;
	widget txtpass;
	widget txtrepass;
	widget rdmale;
	widget rdfemale;
	widget picback;
	widget clipsprite;
	widget sbjob;
	widget status;
};

void new_account(void);
void init_new_account(void);
void newacc_btnclose_press(widget *control, int button, int pressed);
void newacc_btnback_press(widget *control, int button, int pressed);
void newacc_render_sprite(widget *control);
void newacc_sbjob_press(widget *control, int button, int pressed);
void newacc_btncreate_press(widget *control, int button, int pressed);

void newacc_status_button_press(widget *control, int button, int pressed);
void newacc_status_message(char *text);
#endif