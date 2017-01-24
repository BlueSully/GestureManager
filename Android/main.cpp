#include "SDL.h"
#include "GestureManager.h"
#include "BoxObject.h"

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

	SDL_Window *window;
	SDL_Point windowSize;

	SDL_Init(SDL_INIT_VIDEO);

	windowSize.x = 640;
	windowSize.y = 480;

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

	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	BoxObject m_box;

	GestureManager * m_gestureManager;
	m_gestureManager = new GestureManager(windowSize.x, windowSize.y);
	m_gestureManager->createListener(GestureListener::GestureEvent::TAP, &m_box);
	m_gestureManager->createListener(GestureListener::GestureEvent::SWIPE, &m_box);

	while (running)
	{
		m_clock.tick();

		SDL_Event event;

		m_gestureManager->processInput(event);
		TouchEvent * touchEvent = m_gestureManager->getTouchEventData();

		if (touchEvent != nullptr)
		{
			m_box.setPressed(m_box.collisionChecker(touchEvent->getXpos(), touchEvent->getYpos(), 0, 0));
		}


		//switch (m_gestureManager->getEventData())
		//{	
		//	case GestureListener::GestureEvent::TAP:
		//		/**/
		//		break;
		//	case GestureListener::GestureEvent::SWIPE:
		//		m_box.setColour(m_gestureManager->getDebugColour().r, m_gestureManager->getDebugColour().g, m_gestureManager->getDebugColour().b, m_gestureManager->getDebugColour().a);
		//		break;
		//	case GestureListener::GestureEvent::HOLD:
		//		m_box.setColour(m_gestureManager->getDebugColour().r, m_gestureManager->getDebugColour().g, m_gestureManager->getDebugColour().b, m_gestureManager->getDebugColour().a);
		//		break;
		//	case GestureListener::GestureEvent::PINCH:
		//		m_box.setColour(m_gestureManager->getDebugColour().r, m_gestureManager->getDebugColour().g, m_gestureManager->getDebugColour().b, m_gestureManager->getDebugColour().a);
		//		break;
		//}

		m_box.update(m_clock.delta);

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		m_box.draw(renderer);

		m_gestureManager->debugRender(renderer);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}