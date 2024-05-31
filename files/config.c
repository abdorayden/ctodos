#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "config.h"
#include "rdirectorys.h"

bool Is_There_Config(void){
	D_Files df = Init_Dir();
	List_Dir("." , &df);
	for(int i = 0 ; i < df.file_idx ; i++){
		if(strcmp(df.files[i] , CONFIG_NAME) == 0)	return true;
	}
	return false;
}
FILE* Create_Config_File(void){
	if(Is_There_Config())	return NULL;
	return fopen(CONFIG_NAME , "w");
}

void Add_To_Config(FILE* conf_file , Todo_Files* tf , int cnt){
	if(!conf_file)	return;
	for(int i = 0 ; i < cnt ; i++){
		fprintf(conf_file , "+ %s\n" , tf->filename);
		for(int j = 0 ; j < tf->count ; j++){
			fprintf(conf_file , "%s : %s\n" , tf[i].todos[j].done ? "true" : "false" , tf[i].todos[j].str);
		}
	}
}

void dec(char* str , int start , int end){
	if(strlen(str) == 0)	return;
	int i; 
	for(i = 0 ; i < strlen(str) ; i++){
		str[start + i] = str[end + i];
	}
	str[start + i + 1] = '\0';
}

void Load_File(Todo_Files* tf){
	FILE* fd;
	if((fd = fopen(CONFIG_NAME , "r")) == NULL)	return;

	char line[128];
	int i = -1;
	int j = 0;

	while(fgets(line , 128 , fd)){
		if(*line == '+'){
			i++;
			dec(line , 0 , 2);
			strcpy(tf[i].filename,line);
		}
		char* subs = strstr(line , "true");
		if(subs != NULL){
			strcpy(tf[i].todos[j].str , subs);
			tf[i].todos[j].done = true;
			tf[i].todos[j].line_file = i+1;
			tf[i].todos[j].line_term = 0;
		}else{
			strcpy(tf[i].todos[j].str , subs);
			tf[i].todos[j].done = false;
			tf[i].todos[j].line_file = i+1;
			tf[i].todos[j].line_term = 0;
		}
		j++;
	}
}
