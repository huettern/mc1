/* menutree.c
 * Simple LCD menu example based on structs 
 * (c) 2013-2016 by Matthias Meier
 * Released under the GPL V3
 */

#include <stdio.h>
#include "menutree.h"
#include "../p2/header.h"

struct MenuItem_T * activeMenu;		//same as: static struct MenuItem_T activeMenu[];
int activeMenuItem;
static int nrMenuItems;


/**
  * _LoadMenu() - load another menu tree (main- or submenu)
 */
void _LoadMenu(struct MenuItem_T menu[], int nrItems)
{
		activeMenu = menu;
		nrMenuItems = nrItems;
		activeMenuItem = 0;
		ProcessMenu(0); 	//Display first menuitem (on first display line)
}

/**
  * ProcessMenu() - change to next or execute current menu item, then display menu text
 */
void ProcessMenu(char button)
{
	switch (button) {

	case BUTTON_MENU:
		//proceed forward to next menu item (wrap around to first)
		activeMenuItem = (activeMenuItem + 1) % nrMenuItems;
		break;

	case BUTTON_EXECUTE:
	    // if defined, execute the selected menufunction
		if (activeMenu[activeMenuItem].func!=NULL)  
			activeMenu[activeMenuItem].func();
	}
	//display the currently selected menu text
	printf("\n%s",activeMenu[activeMenuItem].text);
}



