#include "Maze.h"
#include <iostream>
#include <fstream>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()


using std::string;
using std::ostream;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::ifstream;
using std::ofstream;

void Maze::makeArray(string file) {
//RANDOM GENERATOR
	int r,s;
	r = random(25) + 5;
	s = random(r);
	

//BUILDING ARRAY OF SIZE r x r OF '#'
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r+1; j++) {
			if (j == (r)) {
				makearray[i][j] = '\n'; //NEXT LINE
			}
			else {
				makearray[i][j] = '#'; 
			}
		}
	}

//ADDING START AND END
	makearray[0][s] = 'S';
	makearray[r-1][r-1] = 'E';

//MAKE MAZE

//WRITING TO FILE
	ofstream outfile;
	outfile.open(file);
	outfile << r << " " << r <<'\n';
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < r + 1; j++) {
			outfile << makearray[i][j];
		}
	}
	outfile.close(); //CLOSE OUTFILE

}

int Maze::random(int num)
{
	srand(time(0));
	int r = (rand() % num);
	return r;
}


Maze::Maze(string file) {

	//couldn't get the random generating maze too work..
	//it is a bit more copmlex than I thought it would be
	//My thoughts on how to get it to work is basically using a function similiar to my 'solve'
	//Print a full array of # then use recursion to print the halls.
	//My biggest problem is I cant get more than one randomly generated number..
	//I know there has to be a way to make multiple but I couldnt figure it out.

	//makeArray(file); //calls the function to make the array

	//READING THE FILE IN
	cout << "File: " << file << endl;
	ifstream infile(file);
	if (!infile) {
		std::cerr << "Cant find " << file << " \n Make sure it is in the right folder\n It should be placed in build folder\n" ;//incase error
		std::exit(-1);
	}

	//MAKE THE ARRAY
	infile >> row >> col;
	cout << "Rows: " << row << " Cols: " << col << endl;
	mazearray[row][col];
	char c;
	infile.get();//parse
	for (int i=0; i < row; i++) {
		for (int j=0; j < col; j++) {

			c = infile.get();
			if (c == '\n') { j = j - 1; }//skips the new line char
			else {
				mazearray[i][j] = c;

				if (c == 'S') { //lets the solve know where to start
					startx = i;
					starty = j;
				}
				if (c == 'E') {
					endx = i;
					endy = j;
				}
			}//end else
		}//end for
		
	}//end for
	infile.close();
}


void Maze::printit() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout << mazearray[i][j];
		}//end for
		cout << endl;
	}//end for
	cout << endl;
}


void Maze::solve() {
	solve(startx, starty);
}


//solve function uses recursive to solve the array
//basically it is going to check to the right, then if that doesn't work to the down, then left, then up.
//If none of those work it will replace the empty spot with V then return and go back to the spot before. 
//It will then look for another empty space. This recursion will continue until it find the letter E for end
void Maze::solve(int x, int y) {

	if (mazearray[x + 1][y] == 'E' || mazearray[x][y + 1] == 'E' || mazearray[x - 1][y] == 'E' || mazearray[x][y - 1] == 'E') {
		complete();
	}
	if (mazearray[x][y + 1] == ' ') { //checks right side
		checkright(x,y+1);
		solve(x,y+1);
	}
	if (mazearray[x + 1][y] == ' ') { //then checks down
		checkdown(x+1,y);
		solve(x+1, y);
	}
	if (mazearray[x][y - 1] == ' ') { //then checks left side3
		checkleft(x,y-1);
		solve(x, y-1);
	}
	if (mazearray[x-1][y] == ' ') {//then checks up
		checkup(x-1,y);
		solve(x-1, y);
	}
	else {
		return; //return if none
	}
}

void Maze::checkright(int startx, int starty) { //this checks the side to the right of current position
	mazearray[startx][starty] = 'V';	
}

void Maze::checkdown(int startx, int starty) { //this checks below the current position
	mazearray[startx][starty] = 'V';
}

void Maze::checkleft(int startx, int starty) { //this checks the side to the left of current position
	mazearray[startx][starty] = 'V';
}

void Maze::checkup(int startx, int starty) { //this checks above current position
	mazearray[startx][starty] = 'V';
}

void Maze::complete() { //this is called if the solve function find the end E.
	printit();
	cout << "You did it!!!!" << endl << endl; 
}