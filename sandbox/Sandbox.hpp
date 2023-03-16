#pragma once

#include <Engine.hpp>

class Sandbox final: public Application
{
public:
	Sandbox(): Application("Sandbox", ENGINE_ROOT_PATH) {}

	void Initialize() override
	{
		mRenderDevice->SetClearColor(Color::BLACK);
		TexturePtr sprite = Texture::Create("assets/image.png");

		mSceneManager.Add(MakeUnique<Scene>("Main"));
		mSceneManager.Switch("Main");
		Scene* scene = mSceneManager.GetCurrent();
		player       = scene->CreateEntity("player");
		player.AddComponent<SpriteRendererComponent>(sprite);
		auto& t = player.GetComponent<TransformComponent>();
		t.Scale = 8.0f;
	}

	void Update(double dt) override
	{
		mRenderDevice->Clear();

		//wp.x = mWindow->GetWidth() * -0.5f + 10.0f;
		//wp.y = mWindow->GetHeight() * -0.5f + 10.0f;
		
		/*mRenderer->DrawBegin(mCamera);

		for(uword i = 0; i < 100; ++i)
			for(uword j = 0; j < 100; ++j)
				mRenderer->DrawCircle({wp.x + (i * 20.0f), wp.y + (j * 20.0f)},
				                      20.0f, Color::BURLYWOOD, 1.0f, 0.095f);
		mRenderer->DrawEnd();*/

		time += dt;
		if(time > 1.0f)
		{
			INFO("{},  {},  {:.4f},  {:.1f}", mRenderer->GetFrameStats().DrawCalls,
			     mRenderer->GetFrameStats().QuadCount, dt, 1.0f / dt);
			time = 0.0f;
		}
	}

private:
	Vec2       wp;
	float      time {0.0f};
	Entity     player;
};
