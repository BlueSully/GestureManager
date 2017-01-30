#include "BoxObject.h"

BoxObject::BoxObject() : MAX_SPEED(1), MIN_SPEED(0.1f), MAX_SIZE(300), MIN_SIZE(15)
{
	m_positionX = 640 / 4;
	m_positionY = 480 / 4;
	m_sizeW = 75;
	m_sizeH = 75;

	m_velocityX = 0;
	m_velocityY = 0;

	m_colour = { 255, 255, 255, 255 };
	m_pressed = false;
}

BoxObject::~BoxObject()
{

}

float BoxObject::getXpos() const
{
	return m_positionX;
}

float BoxObject::getYpos() const
{
	return m_positionY;
}

float BoxObject::getSizeW() const
{
	return m_sizeW;
}

float BoxObject::getSizeH() const
{
	return m_sizeH;
}

float BoxObject::getXvelocity() const
{
	return m_velocityX;
}

float BoxObject::getYvelocity() const
{
	return m_velocityY;
}

void BoxObject::setVelocity(float x, float y)
{
	m_velocityX = x;
	m_velocityY = y;
}

void BoxObject::setColour(int red = 256, int green = 256, int blue = 256, int alpha = 256)
{
	m_colour.r = red;
	m_colour.g = green;
	m_colour.b = blue;
	m_colour.a = alpha;
}

bool BoxObject::collisionChecker(float x, float y, float width, float height)
{
	float aLeft = getXpos();
	float aTop = getYpos();
	float aRight = getXpos() + getSizeW();
	float aBottom = getYpos() + getSizeH();

	float bLeft = x;
	float bTop = y;
	float bRight = x + width;
	float bBottom = y + height;

	//If any of the sides from A are outside of B
	if (aBottom <= bTop)
		return false;

	if (aTop >= bBottom)
		return false;

	if (aRight <= bLeft)
		return false;

	if (aLeft >= bRight)
		return false;

	//If none of the sides from A are outside B
	return true;
}

void BoxObject::boundaryCollision(int worldPosX, int worldPosY, int worldWidth, int worldHeight)
{
	if (getXpos() < 0) 
	{
		m_positionX = 0;
		m_velocityX = -m_velocityX * 0.5f;
	}
	else if (getXpos() + m_sizeW > worldPosX + worldWidth)
	{
		m_positionX = worldPosX + worldWidth - m_sizeW;
		m_velocityX = -m_velocityX * 0.5f;
	}

	if (getYpos() < 0)
	{
		m_positionY = 0;
		m_velocityY = -m_velocityY *0.5f;
	}
	else if (getYpos() + m_sizeH > worldPosY + worldHeight)
	{
		m_positionY = worldPosY + worldHeight - m_sizeH;
		m_velocityY = -m_velocityY *0.5f;
	}

}

void BoxObject::update(float deltaTime)
{
	if (m_velocityX > MAX_SPEED)
	{
		m_velocityX = MAX_SPEED;
	}
	else if (m_velocityX < -MAX_SPEED)
	{
		m_velocityX = -MAX_SPEED;
	}

	if (m_velocityY > MAX_SPEED)
	{
		m_velocityY = MAX_SPEED;
	}
	else if (m_velocityY < -MAX_SPEED)
	{
		m_velocityY = -MAX_SPEED;
	}

	if ((m_velocityX < MIN_SPEED && m_velocityX > 0) ||
		(m_velocityX > -MIN_SPEED && m_velocityX < 0))
	{
		m_velocityX = 0;
	}

	if ((m_velocityY < MIN_SPEED && m_velocityY > 0) ||
		(m_velocityY > -MIN_SPEED && m_velocityY < 0))
	{
		m_velocityY = 0;
	}
	
	m_positionX += (m_velocityX * deltaTime );
	m_positionY += (m_velocityY * deltaTime );

//	std::cout << "vX: " << m_velocityX << " vY: " << m_velocityY << std::endl;
}

void BoxObject::onGesture(GestureListener::GestureEvent evt)
{
	//check if touch is colliding
	if (GestureManager::getInstance()->getTouchEventData(0) != NULL)
	{
		if (collisionChecker(GestureManager::getInstance()->getTouchEventData(0)->getXpos(), GestureManager::getInstance()->getTouchEventData(0)->getYpos(), 0, 0))
		{
			m_pressed = true;
		}
		else
		{
			m_pressed = false;
		}

		switch (evt)
		{
		case GestureListener::GestureEvent::TAP:
			if (m_pressed && GestureManager::getInstance()->getNumberOfTouches() == 1)
			{
				setColour(rand() % 257, rand() % 257, rand() % 257, 256);
			}
			break;
		case GestureListener::GestureEvent::SWIPE:
			if (GestureManager::getInstance()->getNumberOfTouches() == 1)
			{
				m_velocityX += GestureManager::getInstance()->getSwipeData().x;
				m_velocityY += GestureManager::getInstance()->getSwipeData().y;
				setColour(250, 0, 0, 256);
			}
			break;
		case GestureListener::GestureEvent::PINCH:
			if (GestureManager::getInstance()->getNumberOfTouches() == 2)
			{		
				m_sizeW = GestureManager::getInstance()->getPinchScalar();
				m_sizeH = GestureManager::getInstance()->getPinchScalar();

				if (m_sizeW > MAX_SIZE) 
				{
					m_sizeW = MAX_SIZE;
				}
				else if (m_sizeW < MIN_SIZE)
				{
					m_sizeW = MIN_SIZE;
				}

				if (m_sizeH > MAX_SIZE)
				{
					m_sizeH = MAX_SIZE;
				}
				else if (m_sizeH < MIN_SIZE)
				{
					m_sizeH = MIN_SIZE;
				}

				setColour(0, 0, 255, 256);
			}
			break;
		case GestureListener::GestureEvent::HOLD:
			if (GestureManager::getInstance()->getNumberOfTouches() == 1) 
			{
				m_positionX = GestureManager::getInstance()->getTouchEventData(0)->getXpos() - m_sizeW / 2;
				m_positionY = GestureManager::getInstance()->getTouchEventData(0)->getYpos() - m_sizeH / 2;
				m_velocityX = m_velocityY = 0;
				setColour(0, 255, 0, 256);
			}
			break;
		};
	}
}

void BoxObject::draw(SDL_Renderer * renderer)
{
	SDL_Rect sr;
	sr.h = (int)m_sizeH;
	sr.w = (int)m_sizeW;
	sr.x = (int)m_positionX;
	sr.y = (int)m_positionY;

	SDL_SetRenderDrawColor(renderer, m_colour.r, m_colour.g, m_colour.b, m_colour.a);
	SDL_RenderFillRect(renderer, &sr);
}
