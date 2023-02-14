#include <Scene.hpp>

#include <Components.hpp>
#include <Entity.hpp>

Scene::Scene()
{
}

Scene::~Scene()
{
}

Scene::RegistryType& Scene::GetRegistry()
{
	return mRegistry;
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
