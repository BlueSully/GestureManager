#pragma once
#include "SDL.h"
#include "GestureEvent.h"
#include <iostream>
#include<map>
#include<vector>

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
	std::vector<std::pair<TouchEvent*, bool> > m_touches;

	GestureEvent m_currentEvent;

	SDL_Point m_screenSize;

	SDL_Point * m_targetPosition;
	SDL_Point * m_targetSize;

	float m_timeForTapGesture;
	SDL_Rect fillRect;
	SDL_Color m_colour;

	int xMouse, yMouse;

	void swipe();
	void tap();
	void hold();
	void pinchOpen(SDL_Event & evt);
	void pinchClose(SDL_Event & evt);


	void collisionChecker();

public:
	GestureManager();
	GestureManager(int screenWidth, int screenHeight);
	~GestureManager();

	void addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed);
	void removeTouchEvent();

	void processInput(SDL_Event & evt);

	std::pair<GestureEvent, bool> getEventData() const;
	
	void setTargetObject(int &otherXposition, int &otherYposition, int &width, int &height);
	void debugRender(SDL_Renderer * renderer);
	SDL_Color getDebugColour() const;
};