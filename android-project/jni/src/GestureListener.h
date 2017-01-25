#pragma once

struct GestureListener
{
public:
	enum class GestureEvent
	{
		TAP,
		PINCH,
		SWIPE,
		HOLD
	};

	virtual void onGesture(GestureListener::GestureEvent) = 0;
};