#include  "OS_Utils.h"


#ifndef _WIN32
#include <termios.h>
#include <stdio.h>

static struct termios oldT, newT;

/* Initialize new terminal i/o settings */
void initTermios()
{
	tcgetattr(0, &oldT); /* grab old terminal i/o settings */
	newT = oldT; /* make new settings same as old settings */
	newT.c_lflag &= ~ICANON; /* disable buffered i/o */
        newT.c_lflag &= ~ECHO; /* set no echo mode */
	tcsetattr(0, TCSANOW, &newT); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &oldT);
}

/* Read 1 character - echo defines echo mode */
char _getch()
{
	char ch;
	initTermios();
	ch = getchar();
	resetTermios();
	return ch;
}

void enableAnsiEscapes() {
	supportsANSI = true;
	return;
}
#else
#include <windows.h> //ansi escape sequences
#include <conio.h>  //getch()
void enableAnsiEscapes() {

	supportsANSI = false;
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return;
	}
	supportsANSI = true;
	return;
}
#endif

void clearScreen(std::ostream& out) {
	if (supportsANSI) {
		out << ESC << "[2J";  //Clear screen
		out << ESC << "[;H";  //Move cursor to top left corner
	}
	else {
		system("CLS");
	}
}
