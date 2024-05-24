#include<stdio.h>
#include<string.h>
#include<time.h>

#include "clock.h"
#ifndef move_cursor
#define move_cursor(col , row)	\
	printf("\033[%d;%dH" , row, col);
#endif
char* one[6] = {
" __ ",
"/_ |",
" | |",
" | |",
" | |",
" |_|",
};
char* two[6] ={ 
" ___  ",
"|__ \\",
"   ) |",
"  / / ",
" / /_ ",
"|____|",
};
char* three[6] = { 
" ____  ",
"|___ \\",
" __)  |",
"|__ <  ",
" ___) |",
"|____/ ",        
};
char* four[6] = {
" _  _   ",
"| || |  ",
"| || |_ ",
"|__   _|",
"   | |  ",
"   |_|  ",
};

char* five[6] = { 
" _____ ",
"| ____|",
"| |__  ",
"|___ \\",
" ___) |",
"|____/ ",
};
char* six[6] = {
"   __  ",
"  / /  ",
" / /_  ",
"| '_ \\ ",
"| (_) |",
" \\___/ ",
};
char* seven[6] ={
" ______ ",
"|____  |",
"    / / ",
"   / /  ",
"  / /   ",
" /_/    ",
};
char* eight[6] = {
"  ___  ",
" / _ \\",
"| (_) |",
" > _ < ",
"| (_) |",
"\\___/ ",
};
char* nine[6] = {
"  ___  ",
" / _ \\",
"| (_) |",
"\\__, |",
"   / / ",
"  /_/  ",
};
char* zero[6] = { 
"  ___  ",
" / _ \\",
"| | | |",
"| | | |",
"| |_| |",
" \\___/ ",
};
char* column[6] = { 
"   ",
" _ ",
"(_)",
"   ",
" _ ",
"(_)",
};


char* current_time()
{
	time_t rawtime;
	struct tm * timeinfo;
	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return asctime (timeinfo);
}
char t[6];
char* fmt_time(char* f){
	int idx = 0;
	for(int i = 11 ; i < 16 ; i++){
		if(f[i] == '|')	t[idx++] = ':';
		t[idx++] = f[i];
	}
	return t;
}

char* fmt_date(char* f){
	int idx = 0;
	int i;
	for(i = 20 ; f[i] != '\0' ; i++){
		f[i - 9] = f[i];
	}
	f[i - 9] = '\0';
	//while(*f){
	//	d[idx++] = *f;
	//	f++;
	//}
	return f;
}
void handle_and_print(char* fmt , int x , int y){
	if(strlen(fmt) != 5){
		printf("wrong fmt");
		return;
	}
	int idx = 0;
	for (int i = 0 ; i < 6 ; i++){
		move_cursor(x,y);
		while(fmt[idx] != '\0'){
			switch(fmt[idx]){
				case '0':{
					printf("%s\t" , zero[i]);
				}break;
				case '1':{
					printf("%s\t" , one[i]);
				}break;
				case '2':{
					printf("%s\t" , two[i]);
				}break;
				case '3':{
					printf("%s\t" , three[i]);
				}break;
				case '4':{
					printf("%s\t" , four[i]);
				}break;
				case '5':{
					printf("%s\t" , five[i]);
				}break;
				case '6':{
					printf("%s\t" , six[i]);
				}break;
				case '7':{
					printf("%s\t" , seven[i]);
				}break;
				case '8':{
					printf("%s\t" , eight[i]);
				}break;
				case '9':{
					printf("%s\t" , nine[i]);
				}break;
				case ':':{
					printf("%s\t" , column[i]);
				}break;
			}
			idx++;
		}
		y++;
		idx = 0;
	}
}
