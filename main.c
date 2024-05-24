#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "files/bool.h"

#include "files/todo.h"
#include "files/draw.h"

typedef struct winsize Size;
/*
 *		*** Hours  ***  
 *		Date
 *
 *		TODO:
 *		 [ ]
 */

//#define clearscreen()	fputs("\033[2J",stdout);
//#define move_cursor(col , row)	\
//	printf("\033[%d;%dH" , row, col);





Size get_term_size(){
	Size w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w;
}


int main(void){
	Size h = get_term_size();
	int row_max = h.ws_row;
	int col_max = h.ws_col;

	Todo todos[3] = {
		{.str = "ji3an" , .done = false , .line_file = 23 , 30},
		{.str = "nkml" , .done = true , .line_file = 23 , 30},
		{.str = "rgadt" , .done = true , .line_file = 23 , 30},
	};
	clearscreen();
	draw_todos(todos , 3);
	getchar();
	return 0;
}
