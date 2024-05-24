#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "files/bool.h"

#include "files/todo.h"
#include "files/draw.h"
#include "files/keys.h"

typedef struct winsize Size;
/*
 *		*** Hours  ***  
 *		Date
 *
 *		TODO: test it here
 *		 [ ]
 */

Size get_term_size(){
	Size w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w;
}

int count_todos(FILE* file){
	if(file == NULL)	return -1;
	char word[128];
	int lines = 1;
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
			todo[i].line_term = 0;
			i++;
		}
	}
}

    //Term saved_tattr;
    //tcgetattr(STDIN_FILENO, &saved_tattr);

    //// Set the terminal to raw mode
    //set_input_mode();

    //printf("Press any key to see its ASCII code (press 'q' to quit)\n");

    //int ch;
    //while ((ch = getchar()) != 'q') {
    //    printf("The pressed key is: '%c'\n", ch);
    //    printf("ASCII code: %d\n", ch);
    //}

    //// Reset the terminal to original mode before exiting
    //tcsetattr(STDIN_FILENO, TCSANOW, &saved_tattr);

int main(int argc , char** argv){
	if(argc < 2){
		fprintf(stderr , "%s <filename>" , *argv);
		return 1;
	}
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
	//puts(todos[0].str);
	//puts("------------------------");
	//puts(todos[1].str);
	clearscreen();
	draw_todos(todos ,count);
	getchar();
	fclose(filep);
	return 0;
}
