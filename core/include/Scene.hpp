#pragma once

#include <Common.hpp>
#include <Vector2.hpp>

class Entity;
class Application;

class Scene
{
public:
	using RegistryType = entt::registry;

	explicit Scene(String name);
	~Scene();

	void          SetApp(Application* app);
	const String& GetName() const
	{
		return mName;
	}
	RegistryType& GetEntities()
	{
		return mRegistry;
	}

	Entity CreateEntity(const String& name);
	void   DestroyEntity(Entity& entity);
	void   DestroyAllEntities();

	void Initialize();
	void Update(double dt);
	void FixedUpdate(double fdt);
	void Render(double alpha);
	void Resize(vec2ui resolution);

private:
	String       mName;
	Application* mApp;
	RegistryType mRegistry;
};
