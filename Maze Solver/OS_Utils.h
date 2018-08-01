#ifndef OS_UTILS_H
#define OS_UTILS_H
#include <iostream>

#define ESC char(0x1B)

static bool supportsANSI;


void enableAnsiEscapes();
void clearScreen(std::ostream&);

#ifndef _WIN32
#include <termios.h>
#include <stdio.h>
/* Initialize new terminal i/o settings */
void initTermios(int echo);

/* Restore old terminal i/o settings */
void resetTermios(void);

/* Read 1 character - echo defines echo mode */
char _getch();

#else
#include <windows.h> //ansi escape sequences
#include <conio.h>  //getch()
#endif

#endif // !OS_UTILS_H
