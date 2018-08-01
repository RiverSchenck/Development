#include"Maze.h"
#include<iostream>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

using std::endl;
using std::cout;


int main() {

	cout << "MAZE TEST!" << endl << endl;
	
	Maze test1("maze.txt");
	test1.printit();
	test1.solve();


}