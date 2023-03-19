#include "Maze.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stack>

#define WIDTH			1000
#define HEIGHT			1000
#define ROWS			50
#define COLUMNS			50
#define CELL_WIDTH		(WIDTH / ROWS)
#define FPS				512

using namespace std;

void capFramerate(Uint32 starting_tick)
{
	if ((1000 / FPS) > SDL_GetTicks() - starting_tick)
	{
		SDL_Delay(1000 / FPS - (SDL_GetTicks() - starting_tick));
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL)); // Generate random seed

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);

	if (window == NULL)
	{
		cout << "There was an error initializing the window." << endl << SDL_GetError();
		return 1;
	}

	Uint32 starting_tick; // Frame starting tick
	SDL_Event event; // Checks for events
	bool running = true; // Flag for whether the program should continue running
	vector<Maze> maze; // Holds all cells in maze
	stack<Maze*> cellStack; // Holds visited cells

	// Fills maze with all the cells
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			Maze newCell(i, j, CELL_WIDTH);
			maze.push_back(newCell);
		}
	}

	// Initializes the current cell to the starting point
	Maze *current = &maze[0]; 
	current->markVisited();
	cellStack.push(current);

	
	while (running)
	{
		starting_tick = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}

		if (!running)
		{
			break;
		}

		// Gets next cell to visit
		int next = current->checkNeighbors(maze, *current, ROWS, COLUMNS);

		if (next != -1)
		{
			// Choose the random next cell
			Maze& nextCell = maze[next];

			// Push current cell to stack
			cellStack.push(current);

			// Remove walls between current and next
			current->removeWalls(nextCell);

			// Make next cell current
			current = &nextCell;
			current->markVisited();
		}
		else if (!cellStack.empty())
		{
			// Make previous visited cell the current cell and pop from stack
			current = cellStack.top();
			cellStack.pop();
		}

		// Draws the cells in the maze
		for (Uint32 i = 0; i < maze.size(); i++)
		{
			if (!maze[i].isVisited())
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			}

			SDL_Rect rect{ maze[i].getX() * CELL_WIDTH, maze[i].getY() * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH };
			SDL_RenderFillRect(renderer, &rect);

			// Set drawing color to black and display walls for each cell
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			maze[i].display(renderer);
		}
		
		// Displays the current cell position in the program as a grey object
		SDL_SetRenderDrawColor(renderer, 55, 55, 55, SDL_ALPHA_OPAQUE);

		int xHead = current->getX() * CELL_WIDTH;
		int yHead = current->getY() * CELL_WIDTH;

		SDL_Rect rect{ xHead, yHead, CELL_WIDTH, CELL_WIDTH };
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);

		// Clears the frame
		SDL_RenderClear(renderer);

		capFramerate(starting_tick);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
