#include "Sandbox.hpp"

UniquePtr<Application> CreateApp()
{
	return MakeUnique<Sandbox>();
}

Sandbox::Sandbox(): Application("Sandbox", ENGINE_ROOT_PATH)
{
}

void Sandbox::Initialize()
{
	mRenderDevice->SetClearColor(Color::BLACK);
	TexturePtr sprite = Texture::Create("assets/image.png");

	mSceneManager.Add(MakeUnique<Scene>("Main"));
	mSceneManager.Switch("Main");
	Scene* scene = mSceneManager.GetCurrent();

	{
		Entity player = scene->CreateEntity("player");
		player.AddComponent<SpriteRendererComponent>(sprite);
		auto& t = player.GetComponent<TransformComponent>();
		t.Scale = 8.0f;
	}
	// Vec2 wp;
	// wp.x = mWindow->GetWidth() * -0.5f + 10.0f;
	// wp.y = mWindow->GetHeight() * -0.5f + 10.0f;

	{
		Entity circle = scene->CreateEntity("circle0");
		circle.AddComponent<CircleRendererComponent>();
		auto& t      = circle.GetComponent<TransformComponent>();
		t.Position.x = 100;
		auto& s      = circle.GetComponent<CircleRendererComponent>();
		s.Smoothness = 0.1f;
		s.Color      = Color::INDIGO;
	}

	{
		Entity circle = scene->CreateEntity("circle1");
		circle.AddComponent<CircleRendererComponent>();
		auto& t      = circle.GetComponent<TransformComponent>();
		t.Position.x = -100.0f;
		auto& s      = circle.GetComponent<CircleRendererComponent>();
		s.Smoothness = 0.1f;
		s.Color      = Color::YELLOW_GREEN;
	}

	{
		Entity circle = scene->CreateEntity("circle2");
		circle.AddComponent<CircleRendererComponent>();
		auto& t      = circle.GetComponent<TransformComponent>();
		t.Position.y = 100.0f;
		auto& s      = circle.GetComponent<CircleRendererComponent>();
		s.Smoothness = 0.1f;
		s.Color      = Color::CHOCOLATE;
	}

	{
		Entity circle = scene->CreateEntity("circle3");
		circle.AddComponent<CircleRendererComponent>();
		auto& t      = circle.GetComponent<TransformComponent>();
		t.Position.y = -100.0f;
		auto& s      = circle.GetComponent<CircleRendererComponent>();
		s.Smoothness = 0.1f;
		s.Color      = Color::DEEP_PINK;
	}
}

void Sandbox::Update(double dt)
{
	mRenderDevice->Clear();

	time += dt;
	if(time > 1.0f)
	{
		INFO("%u,  %u,  %.4f,  %.1f", mRenderer->GetFrameStats().DrawCalls,
		     mRenderer->GetFrameStats().QuadCount, dt, 1.0f / dt);
		time = 0.0f;
	}
}
