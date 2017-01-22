#include "SDL.h"
#include "GestureManager.h"
#include <stdio.h>
#include <iostream>

using namespace std;

struct Clock
{
	uint32_t last_tick_time = 0;
	uint32_t delta = 0;

	void tick()
	{
		uint32_t tick_time = SDL_GetTicks();
		delta = tick_time - last_tick_time;
		last_tick_time = tick_time;
	}
};

int main(int argc, char* argv[]) 
{
	bool running = true;

	SDL_Window *window;                    // Declare a pointer
	Clock m_clock;
	GestureManager m_gestureManager;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	SDL_Color colour = { 255, 0, 0, 255};
	SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 4, 480 / 4 };

	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_SHOWN                   // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) 
	{
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	while (running) 
	{
		m_clock.tick();
		SDL_Event event;

		m_gestureManager.processInput(event);
		m_gestureManager.collisionChecker(fillRect.x, fillRect.y, fillRect.w, fillRect.h);

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &fillRect);
		m_gestureManager.debugRender(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}