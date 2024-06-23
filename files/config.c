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
FILE* Create_Config_File(){
	if(Is_There_Config())	return NULL;
	return fopen(CONFIG_NAME , "wb");
}

void Add_To_Config(Todo_Files* tf , int cnt){
	FILE* conf_file = fopen(CONFIG_NAME , "wb");
	if(!conf_file){
		fprintf(stderr , "[ERROR] %s", strerror(errno));
		return;
	}
	fwrite(tf , sizeof(Todo_Files) , cnt , conf_file);
	fclose(conf_file);
}

void Load_File(Todo_Files* tf , int* count_files , int k){
	(void)k;
	FILE* fd = fopen(CONFIG_NAME , "rb");
	if(fd == NULL){
		fprintf(stderr , "ERROR : %s" , strerror(errno));
		return;
	}
	fread(tf , sizeof(Todo_Files) , *count_files , fd);
	fclose(fd);
}
