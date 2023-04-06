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
	TexturePtr sprite = Texture::Create("assets/image.png");

	mSceneManager.Add(MakeUnique<Scene>("Main"));
	mSceneManager.Switch("Main");
	Scene* scene = mSceneManager.GetCurrent();

	mWindow->KeySignal.Connect(this, &Sandbox::KeyboardInput);
	mWindow->MouseSignal.Connect(this, &Sandbox::MouseInput);
	mWindow->CursorSignal.Connect(this, &Sandbox::CursorInput);
	mWindow->ScrollSignal.Connect(this, &Sandbox::ScrollInput);

	{
		player = scene->CreateEntity("player");
		player.AddComponent<SpriteRendererComponent>(sprite);
		auto& t = player.GetComponent<TransformComponent>();
		t.Scale = 8.0f;
	}

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

	auto& t = player.GetComponent<TransformComponent>();
	t.Position += pos * dt;

	time += dt;
	if(time > 1.0f)
	{
		INFO("%u,  %u,  %.4f,  %.1f", mRenderer->GetFrameStats().DrawCalls,
		     mRenderer->GetFrameStats().QuadCount, dt, 1.0f / dt);
		time = 0.0f;
	}
}

bool Sandbox::KeyboardInput(Key press, Key release)
{
	const float speed = 200.0f;
	if(press == Key::Right)
		pos.x = speed;
	if(press == Key::Left)
		pos.x = -speed;
	if(press == Key::Up)
		pos.y = -speed;
	if(press == Key::Down)
		pos.y = speed;

	if(release == Key::Right)
		pos.x = 0;
	if(release == Key::Left)
		pos.x = 0;
	if(release == Key::Up)
		pos.y = 0;
	if(release == Key::Down)
		pos.y = 0;

	if(press == Key::Escape)
		Terminate();

	return true;
}

bool Sandbox::MouseInput(MouseButton press, MouseButton release)
{
	if(press == MouseButton::Right)
		INFO("mouse right pressed");
	if(press == MouseButton::Left)
		INFO("mouse left pressed");
	if(press == MouseButton::Middle)
		INFO("mouse middle pressed");
	if(press == MouseButton::Button4)
		INFO("mouse bt4 pressed");
	if(press == MouseButton::Button5)
		INFO("mouse bt5 pressed");

	return true;
}

bool Sandbox::CursorInput(Vec2 position)
{
	INFO("Cursor (%.2f, %.2f)", position.x, position.y);
	return true;
}

bool Sandbox::ScrollInput(Vec2 offset)
{
	INFO("Scroll (%.2f, %.2f)", offset.x, offset.y);
	return true;
}
