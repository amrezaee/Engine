#include <Scene.hpp>

#include <Application.hpp>
#include <Components.hpp>
#include <Entity.hpp>


Scene::Scene(const String& name): mName(name), mApp(nullptr)
{
}

Scene::~Scene()
{
}

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
	e.AddComponent<CameraComponent>(0.0f, mApp->GetWindow().GetWidth(),
	                                mApp->GetWindow().GetHeight(), 0.0f);
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

	r.DrawBegin(camera.Camera);

	// Draw sprites
	{
		auto group = mRegistry.group<TransformComponent>(
		        entt::get<SpriteRendererComponent>);
		for(auto entity : group)
		{
			auto [transform, sprite] =
			        group.get<TransformComponent, SpriteRendererComponent>(entity);

			Vec2 size = transform.Scale * sprite.Texture->GetResolution();
			size.x *= sprite.FlipX ? -1.0f : 1.0f;
			size.y *= sprite.FlipY ? -1.0f : 1.0f;

			r.DrawRotatedQuad(transform.Position, size, transform.Rotation,
			                  sprite.Texture, sprite.Color);
		}
	}

	// Draw circles
	{
		auto view = mRegistry.view<TransformComponent, CircleRendererComponent>();
		for(auto entity : view)
		{
			auto [transform, circle] =
			        view.get<TransformComponent, CircleRendererComponent>(entity);

			r.DrawCircle(transform.Position, transform.Scale.x * 32.0f, circle.Color,
			             circle.Thickness, circle.Smoothness);
		}
	}

	r.DrawEnd();
}

void Scene::Resize(Vec2ui resolution)
{
	auto  view            = mRegistry.view<CameraComponent>();
	auto& cameraComponent = view.get<CameraComponent>(*view.begin());
	cameraComponent.Camera.SetProjection(0.0f, (float)resolution.x,
	                                     (float)resolution.y, 0.0f);
}
