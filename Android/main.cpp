#include "SDL.h"
#include "GestureManager.h"
#include "BoxObject.h"

#include <stdio.h>
#include <stdlib.h>     
#include <time.h>      
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) 
{
	srand(time(NULL));
	bool running = true;

	uint32_t tick_time = 0;
	uint32_t last_tick_time = 0;
	uint32_t delta = 0;

	SDL_Window *window;
	SDL_Point windowSize;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	windowSize.x = DM.w;
	windowSize.y = DM.h;

	//for desktop debuging purposes
	windowSize.x = 480;
	windowSize.y = 640;


	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		windowSize.x,                      // width, in pixels
		windowSize.y,                      // height, in pixels
		SDL_WINDOW_SHOWN                   // flags 
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

	BoxObject m_box;

	GestureManager * m_gestureManager = m_gestureManager->getInstance();
	m_gestureManager->setScreenSize(windowSize.x, windowSize.y);

	m_gestureManager->createListener(GestureListener::GestureEvent::TAP, &m_box);
	m_gestureManager->createListener(GestureListener::GestureEvent::SWIPE, &m_box);
	m_gestureManager->createListener(GestureListener::GestureEvent::HOLD, &m_box);
	m_gestureManager->createListener(GestureListener::GestureEvent::PINCH, &m_box);

	while (running)
	{
		tick_time = SDL_GetTicks();
		delta = (tick_time - last_tick_time);
		last_tick_time = tick_time;

		SDL_Event event;

		m_gestureManager->processInput(event);

		m_box.update(delta);

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		m_box.draw(renderer);
		m_box.boundaryCollision(0, 0, windowSize.x, windowSize.y);
		m_gestureManager->debugRender(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}