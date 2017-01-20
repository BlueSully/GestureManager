#include "SDL.h"
#include "GestureManager.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) 
{
	bool running = true;

	SDL_Window *window;                    // Declare a pointer

	GestureManager m_gestureManager;
	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	SDL_Color colour = { 255, 0, 0, 255};
	SDL_Rect fillRect = { 640 / 4, 480 / 4, 640 / 4, 480 / 4 };
										   // Create an application window with the following settings:									   
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

	// Setup renderer
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool touchpressed = false;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				printf("Key press detected\n");
				if (colour.b == 255)
				{
					colour = m_gestureManager.getColour();
				}
				else if (colour.r == 255)
				{
					colour.r = 0;
					colour.b = 255;
					colour.g = 0;
				}
				else if (colour.g == 255)
				{
					colour.r = 255;
					colour.b = 0;
					colour.g = 0;
				}
				printf("r:%d g:%d b:%d \n", colour.r, colour.g, colour.b);
				break;
			case SDL_KEYUP:
				printf("Key release detected\n");
				break;
			case SDL_FINGERDOWN:
				touchpressed = true;
				break;
			case SDL_FINGERUP:
				if (touchpressed)
				{
					fillRect.y += 10;
					colour = m_gestureManager.getColour();
					
					printf("r:%d g:%d b:%d \n", colour.r, colour.g, colour.b);
					touchpressed = false;
				}
				break;
			default:
				break;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//Render red filled quad

		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &fillRect);

		//Update screen
		SDL_RenderPresent(renderer);

	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}