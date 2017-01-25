#pragma once
#include "SDL.h"
#include "GestureListener.h"
#include "GestureManager.h"
#include <stdlib.h>     

class BoxObject : public GestureListener
{
private :
	float m_positionX;
	float m_positionY;
	float m_sizeW;
	float m_sizeH;
	const int MAX_SPEED;
	float m_velocityX;
	float m_velocityY;

	SDL_Color m_colour;

	bool m_pressed;
public:
	BoxObject();
	~BoxObject();

	float getXpos() const;
	float getYpos() const;
	float getSizeW() const;
	float getSizeH() const;

	float getXvelocity() const;
	float getYvelocity() const;
	void setVelocity(float x, float y);
	void setColour(int red, int green, int blue, int alpha);

	bool collisionChecker(float x, float y, float width, float height);

	void update(float deltaTime);
	void onGesture(GestureListener::GestureEvent evt);
	void draw(SDL_Renderer * render);
};

