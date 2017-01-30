#pragma once
#include "SDL.h"
#include "GestureListener.h"
#include <iostream>
#include <map>
#include <string>
#include <algorithm>  
#include <vector>

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

struct TouchEventData
{
private:
	float m_Xpos;
	float m_Ypos;
	float m_timePressed;
	int m_touchId;
	bool pressed;
	
public:
	TouchEventData() {}

	TouchEventData(float x, float y, int id, float timePressed) : m_Xpos(x), m_Ypos(y), m_touchId(id), m_timePressed(timePressed)
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
	static GestureManager * m_instance;
	GestureManager();

	std::vector<TouchEventData *> m_touches;
	std::map<GestureListener::GestureEvent, std::vector<GestureListener*>*> m_listeners;

	SDL_Point m_screenSize;

	FloatPoint m_swipeVelocity;

	float m_timeForTapGesture;
	float m_pinchScale;

	int xMouse, yMouse;

	void calculateSwipeVelocity(float endPostionX, float endPostionY, float startPositionX, float startPositionY, float distanceTravelled, float timeTaken);
	void calculatePinchData();

public:
	static GestureManager * getInstance();
	
	~GestureManager();

	void setScreenSize(int screenWidth, int screenHeight);

	void addTouchEvent(int xPosition, int yPosition, int id, float timesincePressed);
	void removeTouchEvent(int index);

	void createListener(GestureListener::GestureEvent evt, GestureListener *listener);
	void dispatchEvent(GestureListener::GestureEvent evt);

	void processInput(SDL_Event & evt);

	TouchEventData * getTouchEventData(int index);
	int getNumberOfTouches() const;
	float getPinchScalar() const;
	FloatPoint getSwipeData() const;

	void debugRender(SDL_Renderer * renderer);
};