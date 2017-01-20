#include "GestureManager.h"

GestureManager::GestureManager()
{
	m_colour.r = 0;
	m_colour.g = 255;
	m_colour.b = 0;
	m_colour.a = 255;
}


GestureManager::~GestureManager()
{
}

SDL_Colour GestureManager::getColour() const
{
	return m_colour;
}
