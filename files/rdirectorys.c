#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "rdirectorys.h"

D_Files Init_Dir(void){
	D_Files l_files = {
		.file_idx = 0 
	};
	return l_files;
}

Err List_Dir(const char* dirname , D_Files* l_files){
	if(!dirname)	return 0;
        DIR *dir = opendir(dirname);
	if(dir == NULL){
		return errno;
	}
	errno = 0;
	Dirent* rdir= readdir(dir);
	while(rdir != NULL){
		if (strcmp(rdir->d_name, ".") == 0 || strcmp(rdir->d_name, "..") == 0 || strcmp(rdir->d_name, ".git") == 0) {
			rdir = readdir(dir);
			continue;
		}
		// file max ...
		if(l_files->file_idx == (MAX_FILE - 1))
			break;
		switch(rdir->d_type){
			case DT_REG : {
				snprintf(l_files->files[l_files->file_idx++], 256, "%s/%s", dirname, rdir->d_name);
			}break;
			case DT_DIR : {
				char sub_dir[128];
                		snprintf(sub_dir, 128, "%s/%s", dirname, rdir->d_name);
				//d[strlen(dirname)] = '/';
				//strcat(d , rdir->d_name);
				return List_Dir( sub_dir , l_files);
			}
		}
		rdir = readdir(dir);
	}

	if(errno != 0){
		if(dir)	
			closedir(dir);

		return errno;
	}
	closedir(dir);
	return 0;
}


