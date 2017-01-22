#include "GestureManager.h"

GestureManager::GestureManager()
{
	m_timeForTapGesture = 250;
	m_colour = SDL_Color{ 255, 255, 255, 255 };
	fillRect = { 0, 0, 50, 50 };
}

GestureManager::GestureManager(int screenWidth, int screenHeight)
{
	m_timeForTapGesture = 250;
	m_screenSize.x = screenWidth;
	m_screenSize.y = screenHeight;
	touchLocation.x = screenWidth / 2;
	touchLocation.y = screenHeight / 2;
	m_colour = SDL_Color{ 255, 255, 255, 255 };
	fillRect = { 0, 0, 50, 50 };
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
	std::cout << "Swipe Event" << std::endl;
	m_colour = SDL_Color { 0, 0, 255, 255 };
}

void GestureManager::tap()
{
	std::cout << "Tap Event" << std::endl;
	m_colour = SDL_Color{ 255, 0, 0, 255 };
}

void GestureManager::hold()
{
	std::cout << "Hold Event" << std::endl;
	m_colour = SDL_Color{ 0, 255, 0, 255 };
}

void GestureManager::pinch() 
{
	std::cout << "Pinch Event" << std::endl;
	m_colour = SDL_Color{ 0, 255, 255, 255 };
}

void GestureManager::addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed)
{
	m_touches.push_back(new TouchEvent(static_cast<float>(xPosition), static_cast<float>(yPosition), id, timesincePressed));

	fillRect.x = xPosition - fillRect.w / 2;
	fillRect.y = yPosition - fillRect.h / 2;
	
	m_touchesDebug.push_back(fillRect);
}

void GestureManager::removeTouchEvent()
{
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		delete m_touches[i];	
	}

	m_touches.clear();
	m_touchesDebug.clear();
}

void GestureManager::processInput(SDL_Event & evt)
{
	float dist = 0;
	float value = 0;

	if (m_touches.size() > 0)
	{
		value = SDL_GetTicks() - m_touches[0]->getTimePressed();
	}


	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&xMouse, &yMouse);
			addTouchEvent(xMouse, yMouse, 0, SDL_GetTicks());
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&xMouse, &yMouse);
			if (value > m_timeForTapGesture)
			{
				hold();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (m_touches.size() > 0)
			{
				SDL_GetMouseState(&xMouse, &yMouse);
				dist = sqrt((xMouse - m_touches[0]->getXpos()) * (xMouse - m_touches[0]->getXpos()) + (yMouse - m_touches[0]->getYpos()) * (yMouse - m_touches[0]->getYpos()));

				std::cout << "dist: " << dist << std::endl;
				std::cout << "time: " << value << std::endl;
				if (value < m_timeForTapGesture && dist < 50)
				{
					tap();
				}
				else if (value < m_timeForTapGesture && dist >= 50)
				{
					swipe();
				}

				removeTouchEvent();
			}
			break;
		case SDL_MULTIGESTURE:
			if (evt.mgesture.dDist > 0 && evt.mgesture.numFingers == 2)
			{ 
				pinch();
			}
			break;
		default:
			break;
		}
	}

	if (m_touchesDebug.size() > 0)
	{
		m_touchesDebug[0].x = xMouse;
		m_touchesDebug[0].y = yMouse;
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
	return false;
}

void GestureManager::debugRender(SDL_Renderer * renderer)
{
	for (size_t i = 0; i < m_touchesDebug.size(); i++)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &m_touchesDebug[i]);
	}
}

SDL_Color GestureManager::getDebugColour() const
{
	return m_colour;
}
