#include "SDL.h"
#include "GestureManager.h"

#include <stdio.h>
#include <stdlib.h>     
#include <time.h>      
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
	srand(time(NULL));
	bool running = true;

	Clock m_clock;

	float positionX = 640 / 4;
	float positionY = 480 / 4;
	float sizeW = 100;
	float sizeH = 100;

	SDL_Window *window;

	SDL_Point windowSize;
	SDL_Point velocity;

	GestureManager * m_gestureManager;

	SDL_Init(SDL_INIT_VIDEO);

	windowSize.x = 640;
	windowSize.y = 480;

	velocity.x = 0;
	velocity.y = 0;

	SDL_Color colour = { 255, 0, 0, 255 };

	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		windowSize.x,                      // width, in pixels
		windowSize.y,                      // height, in pixels
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
		m_clock.tick();

		SDL_Event event;

		m_gestureManager->processInput(event);

		colour = m_gestureManager->getDebugColour();

		m_gestureManager->setTargetObject(positionX, positionY, sizeW, sizeH);

		
		if (m_gestureManager->getEventData() == GestureEvent::SWIPE) 
		{
			velocity = m_gestureManager->getSwipeData();
			cout << "veloX: " << velocity.x << " veloY: " << velocity.y << endl;
		}
	/*	else 
		{
			velocity.x = 10;
			velocity.y = 10;
		}*/

		positionX += (velocity.x * m_clock.delta) * 0.00001f;
		positionY += (velocity.y * m_clock.delta) * 0.00001f;

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		
		SDL_Rect sr;
		sr.h = (int)sizeH;
		sr.w = (int)sizeW;
		sr.x = (int)positionX;
		sr.y = (int)positionY;


		SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
		SDL_RenderFillRect(renderer, &sr);
		m_gestureManager->debugRender(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}