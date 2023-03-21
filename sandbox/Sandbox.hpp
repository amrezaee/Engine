#pragma once

#include <Engine.hpp>

class Sandbox final: public Application
{
public:
	Sandbox();
	void Initialize() override;
	void Update(double dt) override;

private:
	float  time {0.0f};
};
