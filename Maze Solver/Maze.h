#ifndef MAZE_H
#define MAZE_H
#include <iostream>
#include <string>

class Maze {

public:
	int row; //array row
	int col; //array col
	int startx; //S row position
	int starty; //S col position
	int endx; //E row position
	int endy; //E col position
	int count;
	char makearray[100][100];//make array
	char mazearray[100][100]; //maze array


	Maze(std::string);
	void makeArray(std::string);
	int random(int num);
	void checkdown(int startx, int starty);
	void checkright(int startx, int starty);
	void checkup(int startx, int starty);
	void checkleft(int startx, int starty);
	void complete();
	void printit(); //std::ostream& out?
	void solve();
	void solve(int startx, int starty);




};

#endif