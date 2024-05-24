#include <stdio.h>
#include <string.h>

#include "todo.h"

void draw_todos(Todo* todo , int size){
	int y = 3;
	move_cursor(5,y);
	printf("\033[1m+ TODO:\033[0m");
	y++;
	move_cursor(5,y);
	draw_line("-" , strlen("todo: "));
	if(todo == NULL){
		move_cursor(7,y + 1);
		printf("No Todos");
		return;
	}
	y++;
	for(int i = 0 ; i < size ; i++){
		move_cursor(7,y + i);
		printf("- [%s] %s", is_done(todo->done), todo->str);
		todo++;
	}
}
