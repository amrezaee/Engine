#pragma once

#include <Assert.hpp>
#include <Common.hpp>
#include <Scene.hpp>

class Entity
{
public:
	using HandleType = entt::entity;

	Entity() = default;
	Entity(HandleType handle, Scene* scene);
	Entity(const Entity&) = default;

	const String& GetName() const;

	template<typename T>
	bool HasComponent() const
	{
		return mScene->GetEntities().all_of<T>(mHandle);
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		ASSERT(!HasComponent<T>(), "Component already exists");
		T& c = mScene->GetEntities().emplace<T>(mHandle,
		                                        std::forward<Args>(args)...);
		return c;
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& c = mScene->GetEntities().emplace_or_replace<T>(
		        mHandle, std::forward<Args>(args)...);
		return c;
	}

	template<typename T>
	T& GetComponent()
	{
		ASSERT(HasComponent<T>(), "Component does not exist");
		return mScene->GetEntities().get<T>(mHandle);
	}

	template<typename T>
	const T& GetComponent() const
	{
		ASSERT(HasComponent<T>(), "Component does not exist");
		return mScene->GetEntities().get<T>(mHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		ASSERT(HasComponent<T>(), "Component does not exist");
		mScene->GetEntities().remove<T>(mHandle);
	}

	explicit operator HandleType() const;
	explicit operator uword() const;
	explicit operator bool() const;

	bool operator==(const Entity& rhs) const;
	bool operator!=(const Entity& rhs) const;

private:
	HandleType mHandle {entt::null};
	Scene*     mScene {nullptr};
};
