#pragma once

#include <Common.hpp>

#include <entt/entt.hpp>

class Entity;

class Scene
{
public:
	using RegistryType = entt::registry;

	Scene();
	~Scene();

	RegistryType& GetRegistry();

	Entity CreateEntity(const String& name);
	void   DestroyEntity(Entity& entity);
	void   DestroyAllEntities();

private:
	RegistryType mRegistry;
};