/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#ifndef DE_STATUS_H
#define DE_STATUS_H
#include "widget.h"
typedef struct status_t status_t;
typedef enum status_type_t status_type_t;

struct status_t{
	widget btnclose;
	widget lblstatus;
	widget frmmain;
	widget wndstatus;
};

enum status_type_t{
	STATUS_TYPE_SOCKET,
	STATUS_TYPE_LOADER
};

uint8 get_status_type(void);
void set_status_type(uint8 type);
void status(void);
void init_status(char *text);
void init_status_loader(char *text);
void status_btnclose_press(widget *control, int button, int pressed);
void status_message(char *text);
#endif