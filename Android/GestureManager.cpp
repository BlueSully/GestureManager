#include "GestureManager.h"

GestureManager::GestureManager()
{
	m_timeForTapGesture = 400;
}

GestureManager::~GestureManager()
{
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		delete m_touches[i];	
	}
}

void GestureManager::swipe()
{

}

void GestureManager::tap()
{

}

void GestureManager::hold()
{

}

void GestureManager::pinch() 
{

}

void GestureManager::addTouchEvent(float xPosition, float yPosition, int id, float timesincePressed)
{
	m_touches.push_back(new TouchEvent(xPosition, yPosition, id, timesincePressed));

	SDL_Rect fillRect = { 640 / 4, 480 / 4, 5, 5 };
	fillRect.x = xPosition;
	fillRect.y = yPosition;

	m_touchesDebug.push_back(fillRect);
}

void GestureManager::removeTouchEvent()
{
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		float value = SDL_GetTicks() - m_touches[i]->getTimePressed();

		if (value < m_timeForTapGesture)
		{
			std::cout << "Tap Event" << std::endl;
		}

		std::cout << "TimePressed: " << value << " milis : " << val << std::endl;
		delete m_touches[i];	
	}

	m_touches.clear();
	m_touchesDebug.clear();
}

void GestureManager::processInput(SDL_Event & evt)
{
	int xMouse, yMouse;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&xMouse, &yMouse);
			addTouchEvent(xMouse, yMouse, 0, SDL_GetTicks());	
			break;
		case SDL_MOUSEBUTTONUP:
			if (m_touches.size() > 0) 
			{
				removeTouchEvent();
			}
			break;
		case SDL_FINGERDOWN:

			break;
		case SDL_FINGERMOTION:

			break;
		case SDL_FINGERUP:

			break;
		default:
			break;
		}
	}
}

bool GestureManager::collisionChecker(float otherXposition, float otherYposition, float width, float height)
{
	if (m_touches.size() > 0)
	{
		for (int i = 0; i < m_touches.size(); i++)
		{
			float aLeft = m_touches[i]->getXpos();
			float aTop = m_touches[i]->getYpos();
			float aRight = m_touches[i]->getXpos() + 3;
			float aBottom = m_touches[i]->getYpos() + 3;

			float bLeft = otherXposition;
			float bTop = otherYposition;
			float bRight = otherXposition + width;
			float bBottom = otherYposition + height;

			return (aLeft <= bRight &&
				bLeft <= aRight &&
				aTop <= bBottom &&
				bTop <= aBottom);
		}
	}
}

void GestureManager::debugRender(SDL_Renderer * renderer)
{
	for (size_t i = 0; i < m_touchesDebug.size(); i++)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &m_touchesDebug[i]);
	}
}
