#include <Scene.hpp>

#include <Application.hpp>
#include <Components.hpp>
#include <Entity.hpp>


Scene::Scene(String name)
        : mName(std::move(name)),
          mApp(nullptr)
{
}

Scene::~Scene() = default;

void Scene::SetApp(Application* app)
{
	if(app)
		mApp = app;
	else
		WARN("app is null");
}

Entity Scene::CreateEntity(const String& name)
{
	Entity e {mRegistry.create(), this};

	e.AddComponent<TagComponent>(name.empty() ? "entity" : name);
	e.AddComponent<TransformComponent>();

	return e;
}

void Scene::DestroyEntity(Entity& entity)
{
	mRegistry.destroy(entt::entity(entity));
}

void Scene::DestroyAllEntities()
{
	mRegistry.clear();
}

void Scene::Initialize()
{
	Entity e = CreateEntity("Camera");
	e.AddComponent<CameraComponent>(
	        0.0f, mApp->GetWindow().GetWidth(), mApp->GetWindow().GetHeight(), 0.0f);
}

void Scene::Update(double dt)
{
	(void)dt;
}

void Scene::FixedUpdate(double fdt)
{
	(void)fdt;
}

void Scene::Render(double alpha)
{
	(void)alpha;
	Renderer& r = mApp->GetRenderer();

	auto camera_view = mRegistry.view<TransformComponent, CameraComponent>();
	auto [camera_transform, camera] =
	        camera_view.get<TransformComponent, CameraComponent>(
	                *camera_view.begin());

	r.DrawBegin(camera.Camera.GetViewProjection());

	// Draw sprites
	{
		auto group = mRegistry.group<TransformComponent>(
		        entt::get<SpriteRendererComponent>);
		for(auto entity : group)
		{
			auto [transform, sprite] =
			        group.get<TransformComponent, SpriteRendererComponent>(entity);

			vec2 size = transform.Scale * sprite.Texture->GetResolution();
			size.x *= sprite.FlipX ? -1.0f : 1.0f;
			size.y *= sprite.FlipY ? -1.0f : 1.0f;

			r.SetColor(sprite.Color);
			r.DrawRotatedQuad(
			        sprite.Texture, transform.Position, size, transform.Rotation);
		}
	}

	// Draw circles
	{
		auto view = mRegistry.view<TransformComponent, CircleRendererComponent>();
		for(auto entity : view)
		{
			auto [transform, circle] =
			        view.get<TransformComponent, CircleRendererComponent>(entity);

			r.SetColor(circle.Color);
			r.DrawCircle(transform.Position,
			             transform.Scale.x * 32.0f,
			             circle.Thickness,
			             circle.Smoothness);
		}
	}

	r.DrawEnd();
}

void Scene::Resize(vec2ui resolution)
{
	auto  view            = mRegistry.view<CameraComponent>();
	auto& cameraComponent = view.get<CameraComponent>(*view.begin());
	cameraComponent.Camera.SetProjection(
	        0.0f, (float)resolution.x, (float)resolution.y, 0.0f);
}
