/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
******************************************************************************/
#include "GL/glew.h"
#include "widget.h"
#include "newaccount.h"
#include "controls.h"
#include "render.h"
#include "clienttcp.h"
#include "bool.h"
#include "globals.h"
#include "general.h"
#include "function.h"
#include "winsocket.h"

new_account_t gui;
sprite box;

float time1;
float lpstimer = 0;
uint8 set = 1;
char *string_newacc;
sbool update_status = FALSE;

void new_account(void)
{
	uint32 time;
	uint32 lpstimer1 = 0;
	uint32 lps = 0;
	int running = TRUE;

	draw_state_reset();

	while(running){
		clear_screen(0,0,0,255);

		widget_manager(widget_get_uip()->root);

		//Clear information from last draw
		glFlush();
		glfwSwapBuffers(get_the_window());
		glfwPollEvents();

		if(update_status){
			status_box_text(&gui.status,string_newacc);
			widget_show(&gui.status);
			widget_manual_focused(&gui.status);
			update_status = FALSE;
		}

		_sleep(30);

		// Check if ESC key was pressed or window was closed
		running = is_window_open();
		if(get_menu_state() != MENU_STATE_CREATE)
			break;
	}
}

void init_new_account(void)
{
	//window
	create_window(&gui.wndnewaccount, NULL, 0, 0, 600, 800, 600, 800,"menuback.png", NULL);
	create_frame(&gui.frmmain, &gui.wndnewaccount,0,0,60,800,FALSE, TRUE);
	//buttons
	create_button(&gui.btnclose,&gui.frmmain,750,10,34,34,34,34,"closebutton.png", NULL);
	create_button(&gui.btnback,&gui.wndnewaccount,196,438,50,180,50,180,"smallbutton.png", NULL);
	create_button(&gui.btncreate,&gui.wndnewaccount,423,438,50,180,50,180,NULL, &gui.btnback);
	//text boxs
	create_stextbox(&gui.txtname,&gui.wndnewaccount,261,123,375,37,4,3,375,37,0,0,0,120,32,2,FALSE,"textbox.png",NULL);
	create_stextbox(&gui.txtpass,&gui.wndnewaccount,261,168,375,37,4,3,375,37,0,0,0,120,32,2,TRUE,NULL,&gui.txtname);
	create_stextbox(&gui.txtrepass,&gui.wndnewaccount,261,212,375,37,4,3,375,37,0,0,0,120,32,2,TRUE,NULL,&gui.txtname);
	create_stextbox(&gui.txtusername,&gui.wndnewaccount,261,255,375,37,4,3,375,37,0,0,0,120,32,2,FALSE,NULL,&gui.txtname);
	//button labels
	create_label(&gui.lblback, &gui.btnback,55,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"BACK");
	create_label(&gui.lblcreate, &gui.btncreate,40,10,80,25,0,0,0,120,FALSE,2,8,FALSE,"CREATE");
	//text box labels
	create_label(&gui.lblpass, &gui.wndnewaccount,168,176,60,18,0,0,0,120,FALSE,1,9,FALSE,"Password");
	create_label(&gui.lblname, &gui.wndnewaccount,198,131,120,25,0,0,0,120,FALSE,1,9,FALSE,"Login");
	create_label(&gui.lblrepass, &gui.wndnewaccount,181,217,120,25,0,0,0,120,FALSE,1,9,FALSE,"Confirm");
	create_label(&gui.lblusername, &gui.wndnewaccount,196,261,120,25,0,0,0,120,FALSE,1,9,FALSE,"Name");
	//scroll bar labels
	create_label(&gui.lbljob, &gui.wndnewaccount,494,324,120,25,0,0,0,120,FALSE,1,9,FALSE,"Job:");
	create_label(&gui.lbljobname, &gui.wndnewaccount,533,324,120,25,0,0,0,120,FALSE,1,9,FALSE,job(0)->name);
	//Job stat labels
	create_label(&gui.lblstrlbl, &gui.wndnewaccount,340,324,120,25,0,0,0,120,FALSE,1,9,FALSE,"Strength:");
	create_label(&gui.lbldeflbl, &gui.wndnewaccount,340,345,120,25,0,0,0,120,FALSE,1,9,FALSE,"Defense:");
	create_label(&gui.lblmagiclbl, &gui.wndnewaccount,340,367,120,25,0,0,0,120,FALSE,1,9,FALSE,"Magic:");
	create_label(&gui.lblspeedlbl, &gui.wndnewaccount,340,389,120,25,0,0,0,120,FALSE,1,9,FALSE,"Speed:");
	//radio labels
	create_label(&gui.lblsexm, &gui.wndnewaccount,517,371,120,25,0,0,0,120,FALSE,1,9,FALSE,"Male");
	create_label(&gui.lblsexf, &gui.wndnewaccount,517,392,120,25,0,0,0,120,FALSE,1,9,FALSE,"Female");
	//Job stat amount labels
	create_label(&gui.lblstr, &gui.wndnewaccount,423,324,120,25,0,0,0,120,FALSE,1,9,FALSE,int_to_string(job(0)->stat[STAT_STRENGTH]));
	create_label(&gui.lbldef, &gui.wndnewaccount,423,345,120,25,0,0,0,120,FALSE,1,9,FALSE,int_to_string(job(0)->stat[STAT_DEFENSE]));
	create_label(&gui.lblmagic, &gui.wndnewaccount,423,367,120,25,0,0,0,120,FALSE,1,9,FALSE,int_to_string(job(0)->stat[STAT_MAGIC]));
	create_label(&gui.lblspeed, &gui.wndnewaccount,423,389,120,25,0,0,0,120,FALSE,1,9,FALSE,int_to_string(job(0)->stat[STAT_SPEED]));
	//radio buttons
	create_radio(&gui.rdmale, &gui.wndnewaccount, 494,372,16,16,16,16,TRUE,"radio.png",NULL,NULL);
	create_radio(&gui.rdfemale, &gui.wndnewaccount, 494,392,16,16,16,16,FALSE,NULL,&gui.rdmale,&gui.rdmale);
	//scrollbars
	create_hscrollbar(&gui.sbjob, &gui.wndnewaccount,494,346,18,100,18,18,18,100,0,2,"sbhback.png","arrowleft.png","arrowright.png","sbhbar.png",NULL);
	//picture box's
	create_picturebox(&gui.picback, &gui.wndnewaccount,219,325,80,80,80,80,comb_2str(GUI_PATH, "spriteback.png"),NULL);
	create_clipbox(&gui.clipsprite,&gui.picback,8,8,64,64,32,32);
	create_statusbox(&gui.status,NULL,(get_screen_width()/2) - 189,(get_screen_height()/2) - 85,"test",NULL, NULL);
	status_box_set_click_event(&gui.status,&newacc_status_button_press);
	//GUI FUNCTION,
	set_scrollbar_buttons(&gui.sbjob, &newacc_sbjob_press);
	gui.clipsprite.draw = &newacc_render_sprite;
	gui.btnclose.mousepress = &newacc_btnclose_press;
	gui.btnback.mousepress = &newacc_btnback_press;
	gui.btncreate.mousepress = &newacc_btncreate_press;
	gui.lblback.action |= WIDGET_CAN_CLICK_BEHIND;
	gui.lblcreate.action |= WIDGET_CAN_CLICK_BEHIND;
	widget_manual_focused(&gui.wndnewaccount);
	widget_hide(&gui.status);

	box.height = 32;
	box.width = 32;
	box.imgpos.x = 32 *5;
	box.imgpos.y = 0;
	box.sizex = 64;
	box.sizey = 64;
	box.pos.x = gui.clipsprite.actualpos.x;
	box.pos.y = gui.clipsprite.actualpos.y;
	load_image(get_path(SPRITE_PATH,job(0)->sprite,IMAGE_ENDING), &box.img);
	create_sprite_vertex_buffer(&box);
}

void newacc_btnclose_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_EXIT);
}

void newacc_btnback_press(widget *control, int button, int pressed)
{
	set_menu_state(MENU_STATE_MAIN);
}

void newacc_btncreate_press(widget *control, int button, int pressed)
{
	textbox *name = (textbox *)gui.txtname.shown.data[0]->control;
	textbox *password = (textbox *)gui.txtpass.shown.data[0]->control;
	textbox *password2 = (textbox *)gui.txtrepass.shown.data[0]->control;
	textbox *charname = (textbox *)gui.txtusername.shown.data[0]->control;
	uint8 sex;

	if(get_str_size(name->string->data) < MIN_NAME_LENGTH){
		newacc_status_message("Username Name must be 4 or more syllables");
		return;
	}

	if(get_str_size(password->string->data) < MIN_PASS_LENGTH){
		newacc_status_message("Password Name must be 4 or more syllables");
		return;
	}

	if(get_str_size(password2->string->data) < MIN_PASS_LENGTH){
		newacc_status_message("Passwords Must Both be filled in.");
		return;
	}

	if(!comp_str(password->string->data,password2->string->data)){
		newacc_status_message("Passwords Do Not Match.");
		return;
	}

	if(get_str_size(charname->string->data) < MIN_NAME_LENGTH){
		newacc_status_message("Character Name must be 4 or more syllables");
		return;
	}

	if(gui.rdmale.action |= WIDGET_CHECKED)
		sex = 0;

	if(gui.rdfemale.action |= WIDGET_CHECKED)
		sex = 1;

	if(isconnected())
		send_new_account(name->string->data,charname->string->data,password->string->data,gui.sbjob.value,sex);
	else
		newacc_status_message("Not Connected To The Server");
}

void newacc_sbjob_press(widget *control, int button, int pressed)
{
	update_label_string(&gui.lbljobname, job(control->parent->value)->name);
	update_label_string(&gui.lblstr, int_to_string(job(control->parent->value)->stat[STAT_STRENGTH]));
	update_label_string(&gui.lbldef, int_to_string(job(control->parent->value)->stat[STAT_DEFENSE]));
	update_label_string(&gui.lblmagic, int_to_string(job(control->parent->value)->stat[STAT_MAGIC]));
	update_label_string(&gui.lblspeed, int_to_string(job(control->parent->value)->stat[STAT_SPEED]));

	load_image(get_path(SPRITE_PATH,job(control->parent->value)->sprite,IMAGE_ENDING), &box.img);
}

void newacc_update_clipsprite_pos(widget *control)
{
	box.pos.x = gui.clipsprite.actualpos.x;
	box.pos.y = gui.clipsprite.actualpos.y;

	sprite_update_vector(&box);
}

void newacc_render_sprite(widget *control)
{
	time1 = glfwGetTime();

	if(lpstimer < time1){
		if(set == 1){
			set = 2;
			box.imgpos.x = 32 *3;
			sprite_update_texture_vector(&box);
		}
		else{
			set = 1;
			box.imgpos.x = 32 *5;
			sprite_update_texture_vector(&box);
		}
		lpstimer = time1 + .60;
	}

	draw_sprite(&box);
}

void newacc_status_button_press(widget *control, int button, int pressed)
{
	widget_hide(&gui.status);
	widget_manual_focused(&gui.wndnewaccount);
}

void newacc_status_message(char *text)
{
	if(update_status == FALSE){
		string_newacc = text;
		update_status = TRUE;
	}
}