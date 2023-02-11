#pragma once

#include <Engine.hpp>

class Sandbox final: public Application
{
public:
	Sandbox(): Application("Sandbox", "D:/Amir/Sources/Projects/practice/") {}

	void Initialize() override
	{
		GetRenderDevice().SetClearColor(Color::BLACK);
		mSprite = Texture::Create("assets/image.png");

		mCamera.SetProjection(0.0f, float(GetWindow().GetWidth()),
		                      float(GetWindow().GetHeight()), 0.0f);
		wp.x = GetWindow().GetWidth() * -0.5f + 10.0f;
		wp.y = GetWindow().GetHeight() * -0.5f + 10.0f;
	}

	void Update(double dt) override {}

	void Draw(double dt) override
	{
		GetRenderDevice().Clear();
		GetRenderer().DrawBegin(mCamera);

		for(U32 i = 0; i < 90; ++i)
			for(U32 j = 0; j < 1000; ++j)
				GetRenderer().DrawCircle({wp.x + (i * 20.0f), wp.y + (j * 20.0f)},
				                         20.0f, Color::BURLYWOOD, 1.0f, 0.095f);
		GetRenderer().DrawQuad({0.0f, 0.0f}, mSprite->GetResolution() * 10.0f,
		                       mSprite);

		GetRenderer().DrawEnd();

		time += dt;
		if(time > 0.8f)
		{
			INFO("{},  {},  {},  {}", GetRenderer().GetFrameStats().DrawCalls,
			     GetRenderer().GetFrameStats().QuadCount, dt, 1.0f / dt);
			time = 0.0f;
		}
	}

private:
	Camera     mCamera;
	TexturePtr mSprite;
	Vec2       wp;
	float      time {0.0f};
};
