#include "Maze.h"
#include <cstdlib>
#include <time.h>
#include <stack>
#include <iostream>


Maze::Maze(int newX, int newY, int cellW) : x(newX), y(newY), cellWidth(cellW), visited(false)
{
	walls[0] = true;
	walls[1] = true;
	walls[2] = true;
	walls[3] = true;
}

int Maze::checkNeighbors(std::vector<Maze> &maze, Maze &current, int rows, int cols)
{
	int next = -1;
	int x = current.getX();
	int y = current.getY();

	std::vector<Maze*> neighbors;

	// Checks each neighboring cell if it has been visited
	if (x > 0 && !maze[(x - 1) * cols + y].isVisited())
	{
		neighbors.push_back(&maze[(x - 1) * cols + y]);
	}
	if (x < rows - 1 && !maze[(x + 1) * cols + y].isVisited())
	{
		neighbors.push_back(&maze[(x + 1) * cols + y]);
	}
	if (y > 0 && !maze[x * cols + (y - 1)].isVisited())
	{
		neighbors.push_back(&maze[x * cols + (y - 1)]);
	}
	if (y < cols - 1 && !maze[x * cols + (y + 1)].isVisited())
	{
		neighbors.push_back(&maze[x * cols + (y + 1)]);
	}
	
	// Find random neighbor index
	if (!neighbors.empty())
	{
		int randomIndex = rand() % neighbors.size();
		next = neighbors[randomIndex]->getY() + (neighbors[randomIndex]->getX() * cols);
	}

	return next;
}

void Maze::markVisited()
{
	this->visited = true;
}

void Maze::removeWalls(Maze& cell)
{
	int xDiff = x - cell.x;
	int yDiff = y - cell.y;

	if (xDiff == -1)
	{
		this->removeWall(1);
		cell.removeWall(3);
	}
	if (xDiff == 1)
	{
		this->removeWall(3);
		cell.removeWall(1);
	}
	if (yDiff == -1)
	{
		this->removeWall(2);
		cell.removeWall(0);
	}
	if (yDiff == 1)
	{
		this->removeWall(0);
		cell.removeWall(2);
	}
}

void Maze::removeWall(int w)
{
	this->walls[w] = false;
}


void Maze::display(SDL_Renderer* renderer)
{
	int xCoord = this->x * cellWidth;
	int yCoord = this->y * cellWidth;

	if (this->walls[0]) //Top Wall
	{
		SDL_RenderDrawLine(renderer, xCoord, yCoord , xCoord + this->cellWidth, yCoord);
	}
	if (this->walls[1]) //Right Wall
	{
		SDL_RenderDrawLine(renderer, xCoord + this->cellWidth, yCoord, xCoord + this->cellWidth, yCoord + this->cellWidth);
	}
	if (this->walls[2]) //Bottom Wall
	{
		SDL_RenderDrawLine(renderer, xCoord, yCoord + this->cellWidth, xCoord + this->cellWidth, yCoord + this->cellWidth);
	}
	if (this->walls[3]) //Left wall
	{
		SDL_RenderDrawLine(renderer, xCoord, yCoord, xCoord, yCoord + this->cellWidth);
	}
}

const bool Maze::isVisited()
{
	return this->visited;
}

const int Maze::getX()
{
	return this->x;
}

const int Maze::getY()
{
	return this->y;
}
