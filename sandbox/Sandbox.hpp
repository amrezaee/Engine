#pragma once

#include <Engine.hpp>

class Sandbox final: public Application
{
public:
	Sandbox(): Application("Sandbox", "D:/Amir/Sources/Projects/Engine/") {}

	void Initialize() override
	{
		GetRenderDevice().SetClearColor(Color::BLACK);
		mSprite = Texture::Create("assets/image.png");

		mCamera.SetProjection(0.0f, float(GetWindow().GetWidth()),
		                      float(GetWindow().GetHeight()), 0.0f);

		// GetWindow().SetWindowMode(WindowMode::BorderlessFS);
	}

	void Update(double dt) override
	{
		wp.x = GetWindow().GetWidth() * -0.5f + 10.0f;
		wp.y = GetWindow().GetHeight() * -0.5f + 10.0f;

		GetRenderDevice().Clear();
		GetRenderer().DrawBegin(mCamera);

		for(U32 i = 0; i < 1000; ++i)
			for(U32 j = 0; j < 100; ++j)
				GetRenderer().DrawCircle({wp.x + (i * 20.0f), wp.y + (j * 20.0f)},
				                         20.0f, Color::BURLYWOOD, 1.0f, 0.095f);

		GetRenderer().DrawQuad({0.0f, 0.0f}, mSprite->GetResolution() * 10.0f,
		                       mSprite);

		GetRenderer().DrawEnd();

		time += dt;
		if(time > 1.0f)
		{
			INFO("{},  {},  {:.4f},  {:.1f}",
			     GetRenderer().GetFrameStats().DrawCalls,
			     GetRenderer().GetFrameStats().QuadCount, dt, 1.0f / dt);
			time = 0.0f;
		}
	}

	void OnResize(Vec2ui resolution) override
	{
		mCamera.SetProjection(0.0f, resolution.x, resolution.y, 0.0f);
	}

private:
	Camera     mCamera;
	TexturePtr mSprite;
	Vec2       wp;
	float      time {0.0f};
};
