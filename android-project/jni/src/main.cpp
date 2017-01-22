#include "SDL.h"
#include "GestureManager.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) 
{
	bool running = true;
	float last_tick_time = 0;
	float delta = 0;
	SDL_Window *window;                    // Declare a pointer
	SDL_Point windowSize;
	GestureManager * m_gestureManager;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
	windowSize.x = 640;
	windowSize.y = 480;

	SDL_Color colour = { 255, 0, 0, 255};
	SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 4, 480 / 4 };

	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		windowSize.x,                               // width, in pixels
		windowSize.y,                               // height, in pixels
		SDL_WINDOW_SHOWN                   // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) 
	{
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	m_gestureManager = new GestureManager(windowSize.x, windowSize.y);
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	while (running) 
	{
		float tick_time = static_cast<float>(SDL_GetTicks());
		delta = tick_time - last_tick_time;
		last_tick_time = tick_time;

		SDL_Event event;

		m_gestureManager->processInput(event);
		m_gestureManager->collisionChecker(static_cast<float>(fillRect.x), 
										   static_cast<float>(fillRect.y), 
										   static_cast<float>(fillRect.w), 
										   static_cast<float>(fillRect.h));

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		colour = m_gestureManager->getDebugColour();
		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &fillRect);
		m_gestureManager->debugRender(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}