#include <stdio.h>
#include <string.h>

#include "bool.h"
#include "draw.h"
#include "todo.h"

void draw_todos(Todo* todo ,char* filename , int size , char* color , int pos){
	int y = 16;
	move_cursor(5,y);
	printf("\033[1m+ TODO:\033[0m");
	move_cursor(120,y);
	printf("\033[1m+ File: %s\033[0m" , filename);
	y++;
	move_cursor(5,y);
	draw_line("-" , strlen("todo: ") + 1);
	move_cursor(120,y);
	draw_line("-" , strlen("todo: ") + 3 + strlen(filename));
	y++;
	if(todo == NULL){
		move_cursor(7,y + 1);
		printf("No Todos");
		return;
	}
	y++;
	for(int i = 0 ; i < size ; i++){
		move_cursor(7,y + i);
		printf("- [%s] %s%s%s", is_done(todo->done),pos == todo->line_term ? color:"\033[49m", todo->str , "\033[49m");
		todo++;
	}
}
