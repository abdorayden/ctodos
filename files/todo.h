#ifndef TODO_H_
#define TODO_H_

#include "bool.h"

#define MAX_TODOS 20
#define MAX_TODOS_FILE 10

typedef struct{
	char str[128];  // todo !
	bool done; 	// is done ?
	int line_file;	// line in file
	int line_term;	// line in terminal
}Todo;

typedef struct{
	char filename[128];
	int count;
	Todo todos[MAX_TODOS];

}Todo_Files;

#ifndef clearscreen
#	define clearscreen()	fputs("\033[2J",stdout)
#endif

#ifndef move_cursor
#define move_cursor(col , row)	\
	printf("\033[%d;%dH" , row, col);
#endif

#define is_done(b)	b ? "\033[92mX\033[39m" : " "
void draw_todos(Todo*,char* , int , char* , int);
#endif // TODO_H_
