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
	m_pinchScale = 0;
}

GestureManager::~GestureManager()
{
	//delete m_touch;	
	//m_touch = nullptr;
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

void GestureManager::calculatePinchData()
{
	float dX, dY;

	if (m_touches.size() >= 2)
	{
		dX = m_touches[0]->getXpos() - m_touches[1]->getXpos();
		dY = m_touches[0]->getYpos() - m_touches[1]->getYpos();
	}

	m_pinchScale = sqrt(dX * dX + dY * dY);

	std::cout << "Scale: " << m_pinchScale << std::endl;
	dispatchEvent(GestureListener::GestureEvent::PINCH);
}

void GestureManager::addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed)
{
	m_touches.push_back(new TouchEventData(static_cast<float>(xPosition), static_cast<float>(yPosition), id, timesincePressed));

}
void GestureManager::removeTouchEvent(int index)
{
	m_touches.erase(std::remove(m_touches.begin(), m_touches.end(), m_touches[index]), m_touches.end());
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

	//std::cout << "size: " << m_touches.size() << std::endl;
	while (SDL_PollEvent(&evt))
	{
		SDL_GetMouseState(&xMouse, &yMouse);
		switch (evt.type)
		{
		case SDL_MULTIGESTURE:
			
			break;
		case SDL_FINGERDOWN:
			addTouchEvent(evt.tfinger.x * m_screenSize.x, evt.tfinger.y * m_screenSize.y, (int)evt.tfinger.fingerId, static_cast<float>(SDL_GetTicks()));		
			break;
		case SDL_FINGERMOTION:
			for (int i = 0; i < static_cast<float>(m_touches.size()); i++)
			{
				timePressed = (SDL_GetTicks() - m_touches[i]->getTimePressed());

				if (timePressed > m_timeForTapGesture)
				{
					m_touches[i]->setXpos(static_cast<float>(evt.tfinger.x * m_screenSize.x));
					m_touches[i]->setYpos(static_cast<float>(evt.tfinger.y * m_screenSize.y));

					if (m_touches.size() == 1)
					{
						dispatchEvent(GestureListener::GestureEvent::HOLD);
					}
					else if (m_touches.size() >= 2)//Looking for mulitple finger gestures
					{
						if (m_touches.size() == 2)//look for two finger gestures
						{
							calculatePinchData();
						}
					}
				}
			}
			break;
		case SDL_FINGERUP:
			for (int i = 0; i < static_cast<int>(m_touches.size()); i++)
			{
				timePressed = SDL_GetTicks() - m_touches[i]->getTimePressed();

				if (m_touches.size() == 1)
				{
					dist = sqrt((evt.tfinger.x * m_screenSize.x - m_touches[0]->getXpos()) * (evt.tfinger.x * m_screenSize.x - m_touches[0]->getXpos()) + (evt.tfinger.y * m_screenSize.y - m_touches[0]->getYpos()) * (evt.tfinger.y * m_screenSize.y - m_touches[0]->getYpos()));

					if (timePressed < m_timeForTapGesture && dist < 5)
					{
						dispatchEvent(GestureListener::GestureEvent::TAP);
					}
					else if (timePressed < m_timeForTapGesture && dist >= 20)
					{
						calculateSwipeVelocity(static_cast<float>(evt.tfinger.x * m_screenSize.x), static_cast<float>(evt.tfinger.y * m_screenSize.y), m_touches[0]->getXpos(), m_touches[0]->getYpos(), dist, timePressed);
					}
				}
				removeTouchEvent(i);
			}
			break;
		//case SDL_MOUSEBUTTONDOWN:
		//	addTouchEvent(xMouse, yMouse, (int)evt.tfinger.fingerId, static_cast<float>(SDL_GetTicks()));		
		//	break;
		//case SDL_MOUSEMOTION:
		//	for (int i = 0; i < static_cast<float>(m_touches.size()); i++)
		//	{
		//		timePressed = (SDL_GetTicks() - m_touches[i]->getTimePressed());

		//		if (timePressed > m_timeForTapGesture)
		//		{
		//			m_touches[i]->setXpos(static_cast<float>(xMouse));
		//			m_touches[i]->setYpos(static_cast<float>(yMouse));

		//			if (m_touches.size() == 1)
		//			{
		//				dispatchEvent(GestureListener::GestureEvent::HOLD);
		//			}
		//			else if (m_touches.size() >= 2)//Looking for mulitple finger gestures
		//			{
		//				if (m_touches.size() == 2)//look for two finger gestures
		//				{
		//					calculatePinchData();
		//				}
		//			}
		//		}
		//	}
		//	break;
		//case SDL_MOUSEBUTTONUP:
		//	for (int i = 0; i < static_cast<int>(m_touches.size()); i++)
		//	{
		//		timePressed = SDL_GetTicks() - m_touches[i]->getTimePressed();

		//		if (m_touches.size() == 1)
		//		{		
		//			dist = sqrt((xMouse - m_touches[0]->getXpos()) * (xMouse - m_touches[0]->getXpos()) + (yMouse - m_touches[0]->getYpos()) * (yMouse - m_touches[0]->getYpos()));

		//			if (timePressed < m_timeForTapGesture && dist < 5)
		//			{
		//				dispatchEvent(GestureListener::GestureEvent::TAP);
		//			}
		//			else if (timePressed < m_timeForTapGesture && dist >= 20)
		//			{
		//				calculateSwipeVelocity(static_cast<float>(xMouse), static_cast<float>(yMouse), m_touches[0]->getXpos(), m_touches[0]->getYpos(), dist, timePressed);
		//			}
		//		}
		//		removeTouchEvent(i);
		//	}
		//	break;
		default:
			break;
		}

		/*for (size_t i = 0; i < m_touches.size(); i++)
		{
			std::cout << "ID: " << m_touches[i]->getId() << " dist : " << dist << std::endl;
		}*/
		if (m_touches.size() > 1) 
		{
			m_touches[0]->setXpos(static_cast<float>(xMouse));
			m_touches[0]->setYpos(static_cast<float>(yMouse));
		}
	}
}

TouchEventData * GestureManager::getTouchEventData(int index)
{
	if (m_touches[index] != nullptr)
	{
		return m_touches[index];
	}

	return 0;
}

int GestureManager::getNumberOfTouches() const
{
	return static_cast<int>(m_touches.size());
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
	for (size_t i = 0; i < m_touches.size(); i++)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

		SDL_Rect sr;
		sr.h = (int)5;
		sr.w = (int)5;
		sr.x = (int)m_touches[i]->getXpos();
		sr.y = (int)m_touches[i]->getYpos();

		SDL_RenderFillRect(renderer, &sr);
	}
}

