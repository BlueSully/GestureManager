#include "GestureManager.h"

GestureManager::GestureManager()
{
	m_timeForTapGesture = 250;
}

GestureManager::GestureManager(int screenWidth, int screenHeight) 
{
	m_timeForTapGesture = 250;
	m_screenSize.x = screenWidth;
	m_screenSize.y = screenHeight;
}

GestureManager::~GestureManager()
{
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		delete m_touches[i];	
	}
}

void GestureManager::calculateSwipeVelocity(float endPostionX, float endPostionY, float startPositionX, float startPositionY, float distanceTravelled, float timeTaken)
{
	m_swipeVelocity.x = startPositionX - endPostionX;
	m_swipeVelocity.y = startPositionY - endPostionY;

	float magData = sqrt((m_swipeVelocity.x * m_swipeVelocity.x) + (m_swipeVelocity.y * m_swipeVelocity.y));

	m_swipeVelocity.x = m_swipeVelocity.x / magData;
	m_swipeVelocity.y = m_swipeVelocity.y / magData;


	//sqrt((m_swipeVelocity.x * m_swipeVelocity.x) + (m_swipeVelocity.y * m_swipeVelocity.y))

	m_swipeVelocity.x *= distanceTravelled;
	m_swipeVelocity.y *= distanceTravelled;

	dispatchEvent(GestureListener::GestureEvent::SWIPE);
}

void GestureManager::pinchOpen(SDL_Event & evt)
{
	std::cout << "Pinch Open Event" << std::endl;
	dispatchEvent(GestureListener::GestureEvent::PINCH);
}

void GestureManager::pinchClose(SDL_Event & evt)
{
	std::cout << "Pinch Close Event" << std::endl;

	dispatchEvent(GestureListener::GestureEvent::PINCH);
}

void GestureManager::addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed)
{
	m_touches.push_back(new TouchEvent(static_cast<float>(xPosition), static_cast<float>(yPosition), id, timesincePressed));

	SDL_Rect sr;
	sr.h = (int)5;
	sr.w = (int)5;
	sr.x = (int)xPosition;
	sr.y = (int)yPosition;

	m_touchesDebug.push_back(sr);
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

void GestureManager::createListener(GestureListener::GestureEvent evt, GestureListener *listener)
{
	if (m_listeners.find(evt) == m_listeners.end())
	{
		//if Event not found create new key for it in the map
		m_listeners[evt] = new std::vector<GestureListener*>();
	}

	m_listeners[evt]->push_back(listener);// push back listener
}

void GestureManager::dispatchEvent(GestureListener::GestureEvent evt)
{
	std::string eventName = "dispatching: ";
	if (m_listeners.find(evt) != m_listeners.end())
	{
		/*for (GestureListener * const &listener : *m_listeners[evt])
		{*/
			for (std::vector<GestureListener *>::iterator iter = m_listeners[evt]->begin(); iter != m_listeners[evt]->end(); iter++)
			{
				switch (evt)
				{
				case GestureListener::GestureEvent::TAP:
					eventName += "TAP";
					break;
				case GestureListener::GestureEvent::HOLD:
					eventName += "HOLD";
					break;
				case GestureListener::GestureEvent::SWIPE:
					eventName += "SWIPE";
					break;
				case GestureListener::GestureEvent::PINCH:
					eventName += "PINCH";
					break;
				}
				

				std::cout << eventName << std::endl;
				(*iter)->onGesture(evt);
			}
			

			//listener->onGesture(evt);
		//}
	}
}

void GestureManager::processInput(SDL_Event & evt)
{
	float dist = 0;
	float timePressed = 0;

	if (m_touches.size() > 0)
	{
		timePressed = SDL_GetTicks() - m_touches[0]->getTimePressed();
	}

	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&xMouse, &yMouse);
			addTouchEvent(xMouse, yMouse, evt.tfinger.touchId, SDL_GetTicks());
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&xMouse, &yMouse);
			if (timePressed > m_timeForTapGesture && evt.mgesture.numFingers == 1)
			{
				if (m_touches.size() > 0)
				{
					m_touches[0]->setXpos((float)xMouse);
					m_touches[0]->setYpos((float)yMouse);
				}
				dispatchEvent(GestureListener::GestureEvent::HOLD);
			}
			break;
		case SDL_MOUSEBUTTONUP:	
			SDL_GetMouseState(&xMouse, &yMouse);
			if (m_touches.size() > 0)
			{							
				dist = sqrt((xMouse - m_touches[0]->getXpos()) * (xMouse - m_touches[0]->getXpos()) + (yMouse - m_touches[0]->getYpos()) * (yMouse - m_touches[0]->getYpos()));

				std::cout << "dist: " << dist << std::endl;
				if (timePressed < m_timeForTapGesture && dist < 5)
				{
					dispatchEvent(GestureListener::GestureEvent::TAP);
				}
				else if (timePressed < m_timeForTapGesture && dist >= 20)
				{
					calculateSwipeVelocity(xMouse, yMouse, m_touches[0]->getXpos(), m_touches[0]->getYpos(), dist, timePressed);
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
				}
				else if (evt.mgesture.dDist < 0)
				{
					pinchClose(evt);
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

TouchEvent * GestureManager::getTouchEventData()
{
	if (m_touches.size() > 0)
	{
		return m_touches[0];
	}

	return 0;
}

FloatPoint GestureManager::getSwipeData() const
{
	return m_swipeVelocity;
}

void GestureManager::debugRender(SDL_Renderer * renderer)
{
	for (size_t i = 0; i < m_touchesDebug.size(); i++)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &m_touchesDebug[i]);
	}
}

