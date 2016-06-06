/* menutree.h
 * Simple LCD menu example based on structs 
 * (c) 2013-2016 by Matthias Meier
 * Released under the GPL V3
 */

#ifndef MENUTREE_H
#define MENUTREE_H


//menu function type (a parameterless function with return value int)
typedef void (*Menufunc_T)(void);

// menu data type (a struct with text to show plus corresponding menu function)
struct MenuItem_T {
	char * text;
	Menufunc_T func;					// gleiches ohne Menufunc_T:    void (*func)(void);
} ;

// global varaibles (normally not used)
struct MenuItem_T * activeMenu;
int activeMenuItem;

// function to Process the Menu when a button is pushed
void ProcessMenu(char button);

//Macro to calculate the number of items of a array
#define NrArrayItems(array) (sizeof(array)/sizeof(array[0]))

// macro to easy load a new menu
#define LoadMenu(menu) _LoadMenu(menu, NrArrayItems(menu));

//internal function to load a new menu
void _LoadMenu(struct MenuItem_T menu[], int nrMenuItems);


#endif //MENUTREE_H

