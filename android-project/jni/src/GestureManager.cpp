#include "GestureManager.h"

GestureManager::GestureManager() : m_currentEvent((GestureEvent)-1)
{
	m_timeForTapGesture = 250;

	m_targetPosition = new SDL_Point();
	m_targetSize = new SDL_Point();

	m_colour.r = 255;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;

	fillRect.x = 0;
	fillRect.y = 0;
	fillRect.w = 50;
	fillRect.h = 50;
}

GestureManager::GestureManager(int screenWidth, int screenHeight) : m_currentEvent((GestureEvent)-1)
{
	m_timeForTapGesture = 250;
	m_screenSize.x = screenWidth;
	m_screenSize.y = screenHeight;

	m_targetPosition = new SDL_Point();
	m_targetSize = new SDL_Point();

	m_colour.r = 255;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;

	fillRect.x = 0;
	fillRect.y = 0;
	fillRect.w = 50;
	fillRect.h = 50;
}

GestureManager::~GestureManager()
{
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		delete m_touches[i].first;	
	}
}

void GestureManager::swipe()
{
	std::cout << "Swipe Event" << std::endl;
	m_colour.r = 0;
	m_colour.g = 0;
	m_colour.b = 255;
	m_colour.a = 255;
}

void GestureManager::tap()
{
	std::cout << "Tap Event" << std::endl;	

	m_colour.r = 255;
	m_colour.g = 0;
	m_colour.b = 0;
	m_colour.a = 255;
}

void GestureManager::hold()
{
	std::cout << "Hold Event" << std::endl;

	m_colour.r = 0;
	m_colour.g = 255;
	m_colour.b = 0;
	m_colour.a = 255;
}

void GestureManager::pinchOpen(SDL_Event & evt)
{
	std::cout << "Pinch Open Event" << std::endl;

	m_colour.r = 0;
	m_colour.g = 255;
	m_colour.b = 255;
	m_colour.a = 255;
}

void GestureManager::pinchClose(SDL_Event & evt)
{
	std::cout << "Pinch Close Event" << std::endl;

	m_colour.r = 125;
	m_colour.g = 0;
	m_colour.b = 125;
	m_colour.a = 255;
}

void GestureManager::addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed)
{
	m_touches.push_back(std::make_pair(new TouchEvent(static_cast<float>(xPosition), static_cast<float>(yPosition), id, timesincePressed), false));

	fillRect.x = xPosition - fillRect.w / 2;
	fillRect.y = yPosition - fillRect.h / 2;
	
	m_touchesDebug.push_back(fillRect);
}

void GestureManager::removeTouchEvent()
{
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		delete m_touches[i].first;	
	}

	m_touches.clear();
	m_touchesDebug.clear();
}

void GestureManager::processInput(SDL_Event & evt)
{
	float dist = 0;
	float value = 0;
	//m_currentEvent = (GestureEvent)-1;

	if (m_touches.size() > 0)
	{
		value = SDL_GetTicks() - m_touches[0].first->getTimePressed();
	}

	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&xMouse, &yMouse);
			addTouchEvent(xMouse, yMouse, evt.tfinger.touchId, SDL_GetTicks());
			collisionChecker();
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&xMouse, &yMouse);
			if (value > m_timeForTapGesture && evt.mgesture.numFingers == 1)
			{
				hold();
				m_currentEvent = GestureEvent::HOLD;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (m_touches.size() > 0)
			{
				SDL_GetMouseState(&xMouse, &yMouse);
				dist = sqrt((xMouse - m_touches[0].first->getXpos()) * (xMouse - m_touches[0].first->getXpos()) + (yMouse - m_touches[0].first->getYpos()) * (yMouse - m_touches[0].first->getYpos()));

				/*std::cout << "dist: " << dist << std::endl;
				std::cout << "time: " << value << std::endl;*/

				if (value < m_timeForTapGesture && dist < 50)
				{
					tap();
					m_currentEvent = GestureEvent::TAP;
				}
				else if (value < m_timeForTapGesture && dist >= 50)
				{
					swipe();
					m_currentEvent = GestureEvent::SWIPE;
				}

				removeTouchEvent();
			}
			break;
		case SDL_MULTIGESTURE:
			if (evt.mgesture.numFingers == 2)
			{
				if (evt.mgesture.dDist >= 0)
				{
					pinchOpen(evt);
					m_currentEvent = GestureEvent::PINCH;
				}
				else if (evt.mgesture.dDist < 0)
				{
					pinchClose(evt);
					m_currentEvent = GestureEvent::PINCH;
				}

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

std::pair<GestureEvent, bool> GestureManager::getEventData() const
{
	if (m_touches.size() > 0)
	{
		for (int i = 0; i < m_touches.size(); i++)
		{
			if (m_touches[i].second)
			{
				return std::make_pair(m_currentEvent, m_touches[i].second);
			}
		}
	}
	return std::make_pair((GestureEvent)-1, false);
}

void GestureManager::collisionChecker()
{
	if (m_touches.size() > 0)
	{
		for (int i = 0; i < m_touches.size(); i++)
		{
			float aLeft = m_touches[i].first->getXpos();
			float aTop = m_touches[i].first->getYpos();
			float aRight = m_touches[i].first->getXpos() + 50;
			float aBottom = m_touches[i].first->getYpos() + 50;

			float bLeft = m_targetPosition->x;
			float bTop = m_targetPosition->y;
			float bRight = m_targetPosition->x + m_targetSize->x;
			float bBottom = m_targetPosition->y + m_targetSize->y;

			m_touches[i].second = aLeft <= bRight &&
								  bLeft <= aRight &&
								  aTop <= bBottom &&
								  bTop <= aBottom;
		}
	}
}

void GestureManager::setTargetObject(int & otherXposition, int & otherYposition, int & width, int & height)
{
	m_targetPosition->x = otherXposition;
	m_targetPosition->y = otherYposition;
	m_targetSize->x = width;
	m_targetSize->y = height;
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
