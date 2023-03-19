#include <vector>
#include "SDL.h"

#ifndef MAZE_
#define MAZE_

class Maze
{
private: 
	int x, y, cellWidth;
	bool walls[4];
	bool visited;

public:
	Maze(int newX, int newY, int cellW);
	int checkNeighbors(std::vector<Maze> &maze, Maze &current, int rows, int cols);
	void markVisited();
	void removeWalls(Maze &cell);
	void removeWall(int w);
	void display(SDL_Renderer* renderer);
	const bool isVisited();
	const int getX();
	const int getY();
};
#endif
