#include "GestureManager.h"

GestureManager::GestureManager()
{
	m_colour = SDL_Color{ 0, 255, 0, 255 };
}


GestureManager::~GestureManager()
{
}

SDL_Colour GestureManager::getColour() const
{
	return m_colour;
}
