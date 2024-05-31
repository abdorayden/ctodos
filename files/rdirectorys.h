#ifndef RDIR_H_
#define RDIR_H_

typedef int Err;
typedef struct dirent Dirent;

#define MAX_FILE 10

typedef struct{
	char files[MAX_FILE][256]; // 50 max files
	int file_idx;
}D_Files;

D_Files Init_Dir(void);
Err List_Dir(const char* , D_Files*);

#endif //RDIR_H_
