#include "BoxObject.h"

BoxObject::BoxObject() : MAX_SPEED(100)
{
	m_positionX = 640 / 4;
	m_positionY = 480 / 4;
	m_sizeW = 100;
	m_sizeH = 100;

	m_velocity.x = 0;
	m_velocity.y = 0;

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

void BoxObject::setPressed(bool value)
{
	m_pressed = value;
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

	m_positionX += (m_velocityX * deltaTime);
	m_positionY += (m_velocityY * deltaTime);
}

void BoxObject::onGesture(GestureListener::GestureEvent evt)
{
	switch (evt)
	{
	case GestureListener::GestureEvent::TAP:
		if (m_pressed) 
		{
			setColour(rand() % 257, rand() % 257, rand() % 257, 256);
			m_pressed = false;
		}
		break;
	};
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
