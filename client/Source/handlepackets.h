/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/

#ifndef DE_HANDLEPACKETS_H
#define DE_HANDLEPACKETS_H

#include "error.h"
#include "types.h"
#include "buffer.h"
#include "bool.h"
#include "packetenum.h"

void init_packets(void);
void incomming_packets(buffer_t *data);
void handle_loginok(buffer_t *data);

#endif