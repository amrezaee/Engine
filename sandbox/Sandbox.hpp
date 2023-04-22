#pragma once

#include <Engine.hpp>

class Sandbox final: public Application
{
public:
	Sandbox();
	void Initialize() override;
	void Update(double dt) override;

	bool KeyboardInput(Key press, Key release);
	bool MouseInput(MouseButton press, MouseButton release);
	bool CursorInput(vec2 position);
	bool ScrollInput(vec2 offset);

private:
	float  time {0.0f};
	vec2   pos;
	Entity player;
};
