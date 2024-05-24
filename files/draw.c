#include <stdio.h>
#include <math.h>
#include <string.h>

#include "draw.h"

void draw_line(char* ch , int long_size){
	for(int i = 0 ; i < long_size ; i++){
		printf("%s" , ch);
	}
}


void draw_col(char* ch , int c_x , int c_y , int long_size){
	for(int i = 0 ; i < long_size ; i++){
		printf("%s",ch);
		move_cursor(c_x,c_y + i);
	}
}

void draw_box(char* ch_x , char* ch_y , int msquar , int x , int y){
	if( ch_x == NULL || ch_y == NULL)	return;
	move_cursor(x , y);
	draw_line(ch_x , (int)sqrt(msquar));
	draw_col(ch_y , x , y + 1 , (int)sqrt(msquar));
	draw_col(ch_y , x + ((int)sqrt(msquar) * strlen(ch_x)) , y + 1 , (int)sqrt(msquar) + 1);
	move_cursor(x , y + ((int)sqrt(msquar)));
	draw_line(ch_x , (int)sqrt(msquar));
}
