#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "files/bool.h"

#include "files/todo.h"
#include "files/draw.h"
#include "files/clock.h"

typedef struct winsize Size;
void set_input_mode() {
    struct termios tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO); 
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;             
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void reset_input_mode() {
    struct termios tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

Size get_term_size(){
	Size w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w;
}

int count_todos(FILE* file){
	if(file == NULL)	return -1;
	char word[128];
	int lines = 0;
	// TODO: ...
	while(fscanf(file , "%s" , word) != EOF){
		if(strcmp(word , "TODO:") == 0){
			lines++;
		}
	}
	return lines;
}

void get_todos(FILE* file , Todo* todo){
	if(count_todos(file) < 0)	return ;
	if(todo == NULL)	return; 
	char line[50];
	int i = 0;
	int lines = 0;
	int t = 9;
	rewind(file);
	while((fgets(line , 50 , file)) != NULL){
		++lines;
		char* cur_place = strstr(line , "TODO:");
		if(cur_place != NULL){
			for (int j = 0 ;j < strlen(cur_place) - 5 ; j++ ){
				cur_place[j] = cur_place[j + 5];
			}
			for(int j = 0 ; j < strlen(cur_place) ; j++){
				if(cur_place[j] == '\n'){
					for(int k = j ; k < strlen(cur_place) ; k++){
						cur_place[k] = 0;
					}
				}
			}
			strcpy(todo[i].str , cur_place);
			todo[i].done = false;
			todo[i].line_file = lines;
			todo[i].line_term = t++;
			i++;
		}
	}
}

int main(int argc , char** argv){
	if(argc < 2){
		fprintf(stderr , "%s <filename>" , *argv);
		return 1;
	}
	int x_cursor= 13;
	int y_cursor= 9;
	char* file = *(argv + 1);
	Size h = get_term_size();
	int row_max = h.ws_row;
	int col_max = h.ws_col;
	FILE* filep;
	if((filep = fopen(file , "r")) == NULL){
		fprintf(stderr , "ERROR : %s" , strerror(errno));
		return 1;
	}
	int count = count_todos(filep);
	if(count < 1){
		printf("No TODOS");
		fclose(filep);
		return 1;
	}
	Todo todos[count];
	get_todos(filep , todos);
	clearscreen();
	handle_and_print(fmt_time(current_time()) , 40 , 5);
	move_cursor(25 , 12);
	puts(fmt_date(current_time()));
	draw_todos(todos ,count , "\033[47m" , y_cursor);

    	struct termios saved_tattr;
    	tcgetattr(STDIN_FILENO, &saved_tattr);
	move_cursor(x_cursor,y_cursor);
	printf("\033[?25l");
    	set_input_mode();
    	while (true) {
    		int ch = getchar();
		switch(ch){
			case 'k':{
				if(y_cursor > 9){
					y_cursor--;
					move_cursor(x_cursor , y_cursor);
					clearscreen();
					handle_and_print(fmt_time(current_time()) , 40 , 5);
					move_cursor(25 , 12);
					puts(fmt_date(current_time()));
					draw_todos(todos , count, "\033[47m" , y_cursor);
				}
			}break;
			case 'j':{
				if(y_cursor < row_max){
					y_cursor++;
					move_cursor(x_cursor, y_cursor);
					clearscreen();
					handle_and_print(fmt_time(current_time()) , 40 , 5);
					move_cursor(25 , 12);
					puts(fmt_date(current_time()));
					draw_todos(todos , count, "\033[47m" , y_cursor);
				}

			}break;
			case 10:{
				for(int i = 0 ; i < count ; i++){
					if(y_cursor == todos[i].line_term){
						todos[i].done = true;
						clearscreen();
						handle_and_print(fmt_time(current_time()) , 40 , 5);
						move_cursor(25 , 12);
						puts(fmt_date(current_time()));
						draw_todos(todos , count, "\033[47m" , y_cursor);
					}
				}
			}break;
			case 'q':{
    				tcsetattr(STDIN_FILENO, TCSANOW, &saved_tattr);
				printf("\033[?25l");
				fclose(filep);
				clearscreen();
				exit(0);
			}
		}
    	}
	return 0;
}
