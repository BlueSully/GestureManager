#pragma once
#include "SDL.h"
#include "GestureListener.h"
#include <iostream>
#include<map>
#include <string>
#include<vector>

struct FloatPoint
{
public:
	float x;
	float y;

	FloatPoint operator+(const FloatPoint& b) 
	{
		this->x += b.x;
		this->y += b.y;
		return *this;
	}

	FloatPoint operator-(const FloatPoint& b)
	{
		this->x -= b.x;
		this->y -= b.y;
		return *this;
	}

	FloatPoint operator*(const FloatPoint& b)
	{
		this->x *= b.x;
		this->y *= b.y;
		return *this;
	}

	FloatPoint operator/(const FloatPoint& b)
	{
		this->x /= b.x;
		this->y /= b.y;
		return *this;
	}
};

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

	void setXpos(float value) 
	{
		m_Xpos = value;
	}

	void setYpos(float value)
	{
		m_Ypos = value;
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

	std::map<GestureListener::GestureEvent, std::vector<GestureListener*>*> m_listeners;

	SDL_Point m_screenSize;

	FloatPoint m_swipeVelocity;

	float m_timeForTapGesture;

	int xMouse, yMouse;

	void calculateSwipeVelocity(float endPostionX, float endPostionY, float startPositionX, float startPositionY, float distanceTravelled, float timeTaken);
	void pinchOpen(SDL_Event & evt);
	void pinchClose(SDL_Event & evt);

public:
	GestureManager();
	GestureManager(int screenWidth, int screenHeight);
	~GestureManager();

	void addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed);
	void removeTouchEvent();

	void createListener(GestureListener::GestureEvent evt, GestureListener *listener);
	void dispatchEvent(GestureListener::GestureEvent evt);

	void processInput(SDL_Event & evt);

	TouchEvent * getTouchEventData();

	FloatPoint getSwipeData() const;

	void debugRender(SDL_Renderer * renderer);
};