#pragma once
#include "SDL.h"

class GestureManager
{
private:
	SDL_Color m_colour;

public:
	GestureManager();
	~GestureManager();

	SDL_Colour getColour() const;
};

