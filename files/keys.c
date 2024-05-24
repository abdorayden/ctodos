#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "keys.h"

typedef struct termios Term;

void set_input_mode() {
    Term tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO); 
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;             
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void reset_input_mode() {
    Term tattr;
    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}
