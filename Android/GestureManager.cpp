#include "GestureManager.h"

GestureManager *  GestureManager::m_instance = nullptr;

GestureManager * GestureManager::getInstance()
{
	if (!m_instance)
	{
		m_instance = new GestureManager();
	};

	return m_instance;
}

GestureManager::GestureManager()
{
	m_timeForTapGesture = 250;
	m_touch = 0;
	m_pinchScale = 0;
	m_numberOfTouches = 0;
}

GestureManager::~GestureManager()
{
	delete m_touch;	
	m_touch = nullptr;
	delete m_instance;
}

void GestureManager::setScreenSize(int screenWidth, int screenHeight)
{
	m_screenSize.x = screenWidth;
	m_screenSize.y = screenHeight;
}

void GestureManager::calculateSwipeVelocity(float endPostionX, float endPostionY, float startPositionX, float startPositionY, float distanceTravelled, float timeTaken)
{
	m_swipeVelocity.x = startPositionX - endPostionX;
	m_swipeVelocity.y = startPositionY - endPostionY;

	float magData = sqrt((m_swipeVelocity.x * m_swipeVelocity.x) + (m_swipeVelocity.y * m_swipeVelocity.y));

	m_swipeVelocity.x = -m_swipeVelocity.x / magData;
	m_swipeVelocity.y = -m_swipeVelocity.y / magData;

	float speed = distanceTravelled / timeTaken;

	m_swipeVelocity.x *= speed;
	m_swipeVelocity.y *= speed;

	dispatchEvent(GestureListener::GestureEvent::SWIPE);
}

void GestureManager::pinchOpen(SDL_Event & evt)
{
	std::cout << "Pinch Open Event" << std::endl;
	m_pinchScale = evt.mgesture.dDist;
	dispatchEvent(GestureListener::GestureEvent::PINCH);
}

void GestureManager::pinchClose(SDL_Event & evt)
{
	std::cout << "Pinch Close Event" << std::endl;

	dispatchEvent(GestureListener::GestureEvent::PINCH);
}

void GestureManager::addTouchEvent(int xPosition, int yPosition, float timesincePressed)
{
	m_touch = new TouchEventData(static_cast<float>(xPosition), static_cast<float>(yPosition), timesincePressed);

	SDL_Rect sr;
	sr.h = (int)5;
	sr.w = (int)5;
	sr.x = (int)xPosition;
	sr.y = (int)yPosition;

	m_touchesDebug.push_back(sr);
}

void GestureManager::removeTouchEvent()
{
	delete m_touch;
	m_touch = nullptr;
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
	}
}

void GestureManager::processInput(SDL_Event & evt)
{
	float dist = 0;
	float timePressed = 0;

	if (m_touch != nullptr)
	{
		timePressed = SDL_GetTicks() - m_touch->getTimePressed();		
	}

	while (SDL_PollEvent(&evt))
	{
		SDL_GetMouseState(&xMouse, &yMouse);
		switch (evt.type)
		{
		case SDL_MULTIGESTURE:
			if (evt.mgesture.numFingers >= 2)//Looking for mulitple finger gestures
			{
				if (evt.mgesture.numFingers == 2)//look for two finger gestures
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
			}
			break;
		case SDL_FINGERDOWN:
			addTouchEvent(xMouse, yMouse, static_cast<float>(SDL_GetTicks()));
			m_numberOfTouches++;
			break;
		case SDL_FINGERMOTION:
			if (timePressed > m_timeForTapGesture && m_touch != nullptr)
			{
				if (m_numberOfTouches == 1)
				{
					m_touch->setXpos(static_cast<float>(xMouse));
					m_touch->setYpos(static_cast<float>(yMouse));
					dispatchEvent(GestureListener::GestureEvent::HOLD);
				}
			}
			break;
		case SDL_FINGERUP:
			if (m_touch != nullptr)
			{
				dist = sqrt((xMouse - m_touch->getXpos()) * (xMouse - m_touch->getXpos()) + (yMouse - m_touch->getYpos()) * (yMouse - m_touch->getYpos()));

				if (timePressed < m_timeForTapGesture && dist < 5)
				{
					dispatchEvent(GestureListener::GestureEvent::TAP);
				}
				else if (timePressed < m_timeForTapGesture && dist >= 20)
				{
					calculateSwipeVelocity(static_cast<float>(xMouse), static_cast<float>(yMouse), m_touch->getXpos(), m_touch->getYpos(), dist, timePressed);
				}
				removeTouchEvent();
			}
			if (m_numberOfTouches > 0)
			{
				m_numberOfTouches--;
			}
			break;

		//case SDL_MOUSEBUTTONDOWN:
		//	addTouchEvent(xMouse, yMouse, static_cast<float>(SDL_GetTicks()));
		//	m_numberOfTouches++;
		//	break;
		//case SDL_MOUSEMOTION:
		//	if (timePressed > m_timeForTapGesture && m_touch != nullptr)
		//	{
		//		if (m_numberOfTouches == 1)
		//		{
		//			m_touch->setXpos(static_cast<float>(xMouse));
		//			m_touch->setYpos(static_cast<float>(yMouse));
		//			dispatchEvent(GestureListener::GestureEvent::HOLD);
		//		}
		//	}
		//	break;
		//case SDL_MOUSEBUTTONUP:
		//	if (m_touch != nullptr)
		//	{
		//		dist = sqrt((xMouse - m_touch->getXpos()) * (xMouse - m_touch->getXpos()) + (yMouse - m_touch->getYpos()) * (yMouse - m_touch->getYpos()));

		//		if (timePressed < m_timeForTapGesture && dist < 5)
		//		{
		//			dispatchEvent(GestureListener::GestureEvent::TAP);
		//		}
		//		else if (timePressed < m_timeForTapGesture && dist >= 20)
		//		{
		//			calculateSwipeVelocity(static_cast<float>(xMouse), static_cast<float>(yMouse), m_touch->getXpos(), m_touch->getYpos(), dist, timePressed);
		//		}
		//		removeTouchEvent();
		//	}
		//	if (m_numberOfTouches > 0) 
		//	{
		//		m_numberOfTouches--;
		//	}
		//	break;
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

TouchEventData * GestureManager::getTouchEventData()
{
	if (m_touch != nullptr)
	{
		return m_touch;
	}

	return 0;
}

int GestureManager::getNumberOfTouches() const
{
	return m_numberOfTouches;
}

float GestureManager::getPinchScalar() const
{
	return m_pinchScale;
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

