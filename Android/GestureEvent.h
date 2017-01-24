#pragma once
class GestureListener
{
public:

	enum GestureEvent
	{
		TAP,
		PINCH,
		SWIPE,
		HOLD
	};

	virtual void onGesture(GestureEvent) = 0;
};