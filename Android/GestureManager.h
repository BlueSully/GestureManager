#pragma once
#include "SDL.h"
#include "GestureEvents.h"
#include <iostream>
#include <vector>

struct TouchEvent
{
private:
	float m_Xpos;
	float m_Ypos;
	float m_timePressed;
	int m_touchId;
	bool pressed;


public:
	TouchEvent() {}

	TouchEvent(float x, float y, int id, float timePressed) : m_Xpos(x), m_Ypos(y), m_touchId(id), m_timePressed(timePressed) 
	{
		
	}

	int getId() const
	{
		return m_touchId;
	}

	float getXpos() const
	{
		return m_Xpos;
	}

	float getYpos() const
	{
		return m_Ypos;
	}

	float getTimePressed() const 
	{
		return m_timePressed;
	}
};

class GestureManager
{
private:
	std::vector<SDL_Rect> m_touchesDebug;
	std::vector<TouchEvent*> m_touches;

	float m_timeForTapGesture;

	int xMouse, yMouse;

public:
	GestureManager();
	~GestureManager();

	void swipe();
	void tap();
	void hold();
	void pinch();

	void addTouchEvent(float xPosition, float yPosition, int id, float timesincePressed);
	void removeTouchEvent();

	void processInput(SDL_Event & evt);
	
	bool collisionChecker(float otherXposition, float otherYposition, float width, float height);
	void debugRender(SDL_Renderer * renderer);
};