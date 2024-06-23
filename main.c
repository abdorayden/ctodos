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
#include "files/rdirectorys.h"
#include "files/config.h"

typedef struct winsize Size;

void handle_time(char* x , int y){
	handle_and_print(fmt_time(current_time()) , 40 , 5);
	move_cursor(25 , 12);
	puts(fmt_date(current_time()));
	draw_line(x ,y);
}

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

int count_todos(char* filename){
	FILE* file = fopen(filename , "r");
	if(file == NULL)	return -1;
	char word[128];
	int lines = 0;
	// TODO: ...
	while(fgets(word, 128 ,file ) != NULL){
		if(strstr(word , "TODO:") != NULL){
			lines++;
		}
	}
	fclose(file);
	return lines;
}
void get_todos(char* filename , Todo* todo){
	FILE* file = fopen(filename , "r");
	//if(count_todos(file) < 0)	return ;
	if(todo == NULL || file == NULL)	return; 
	char line[50];
	int i = 0;
	int lines = 0;
	int t = 9;
	rewind(file);
	while((fgets(line , 50 , file)) != NULL){
		++lines;
		char* cur_place = strstr(line , "TODO:");
		if(cur_place != NULL){
			for (size_t j = 0 ;j < strlen(cur_place) - 5 ; j++ ){
				cur_place[j] = cur_place[j + 5];
			}
			for(size_t j = 0 ; j < strlen(cur_place) ; j++){
				if(cur_place[j] == '\n'){
					for(size_t k = j ; k < strlen(cur_place) ; k++){
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
	fclose(file);
}

void Load_Directory(char* direname , Todo_Files* tf , int* x){
	D_Files df = Init_Dir();
	List_Dir(direname , &df);
	int k = 0;
	for(int i = 0 ; i <= df.file_idx;  i++ ){
		int c = count_todos(df.files[i]);
		if(c > 0){
			// the filename has todo lines
			if(k < 10){
				strcpy(tf[k].filename,df.files[i]);
				tf[k].count = c;
				get_todos(df.files[i] , tf[k].todos);
			}
			k++;
		}
	}
	*x = k;
}

int main(int argc , char** argv){
	if(argc < 2){
		fprintf(stderr , "%s <directory>" , *argv);
		return 1;
	}
	int x_cursor= 13;
	int y_cursor= 9;
	char* dir = *(argv + 1);
	Size h = get_term_size();
	//int row_max = h.ws_row; Maybe i will use it later
	int col_max = h.ws_col;
	
	Todo_Files df[MAX_TODOS_FILE];
	int z;
	Load_Directory(dir , df , &z);
	int k = 0;

	clearscreen();
	handle_time("-" ,col_max);
	draw_todos(df[k].todos , df[k].filename ,df[k].count , "\033[47m" , y_cursor);
	//draw_todos(todos ,count , "\033[47m" , y_cursor);

	bool quit = false;
    	struct termios saved_tattr;
    	tcgetattr(STDIN_FILENO, &saved_tattr);
	move_cursor(x_cursor,y_cursor);
	printf("\033[?25l");
    	set_input_mode();
    	while (!quit) {
    		int ch = getchar();
		switch(ch){
			case 'k':{
				if(y_cursor > 9){
					y_cursor--;
					move_cursor(x_cursor , y_cursor);
					clearscreen();
					handle_time("-" ,col_max);
					draw_todos(df[k].todos , df[k].filename ,df[k].count , "\033[47m" , y_cursor);
				}
			}break;
			case 'j':{
				if(y_cursor < (9 + (df[k].count - 1))){
					y_cursor++;
					move_cursor(x_cursor, y_cursor);
					clearscreen();
					handle_time("-" ,col_max);
					draw_todos(df[k].todos , df[k].filename ,df[k].count , "\033[47m" , y_cursor);
				}

			}break;
			case 10:{
				for(int i = 0 ; i < df[k].count ; i++){
					if(y_cursor == df[k].todos[i].line_term){
						df[k].todos[i].done = true;
						clearscreen();
						handle_time("-" ,col_max);
						draw_todos(df[k].todos , df[k].filename ,df[k].count , "\033[47m" , y_cursor);
					}
				}
			}break;
			case 9 :{
				if(k == (z - 1))	k=0;
				else	k++;
				move_cursor(x_cursor, y_cursor);
				clearscreen();
				handle_time("-" ,col_max);
				draw_todos(df[k].todos , df[k].filename ,df[k].count , "\033[47m" , y_cursor);
			}break;
			case 'q':{
				quit = true;
			}
		}
    	}
    	tcsetattr(STDIN_FILENO, TCSANOW, &saved_tattr);
	clearscreen();
	printf("\033[?25h");
	return 0;
}
