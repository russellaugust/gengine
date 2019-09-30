//
// Console.h
//
// Clark Kromenaker
//
// In-game console where some log data is output
// and sheep commands can be entered.
//
#pragma once
#include "Actor.h"

class RectTransform;
class UICanvas;

class ConsoleUI : public Actor
{
public:
	ConsoleUI(bool mini);
	
protected:
	void OnUpdate(float deltaTime) override;
	
private:
	// The console's canvas, which renders the UI.
	UICanvas* mCanvas = nullptr;
	
	RectTransform* mBackgroundTransform = nullptr;
};
