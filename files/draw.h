#ifndef DRAW_H_
#define DRAW_H_

#ifndef clearscreen
#	define clearscreen()	fputs("\033[2J",stdout);
#endif

#ifndef move_cursor
#define move_cursor(col , row)	\
	printf("\033[%d;%dH" , row, col);
#endif

void draw_line(char*, int);
void draw_col(char*, int, int, int);
void draw_box(char*, char*, int, int, int);

#endif //DRAW_H_
