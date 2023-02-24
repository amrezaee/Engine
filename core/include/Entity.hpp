#pragma once

#include <Common.hpp>

#include <Assert.hpp>
#include <Scene.hpp>

#include <entt/entt.hpp>

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
		return mScene->GetRegistry().all_of<T>(mHandle);
	}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		ASSERT(!HasComponent<T>(), "Component already exists");
		T& c = mScene->GetRegistry().emplace<T>(mHandle,
		                                        std::forward<Args>(args)...);
		return c;
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& c = mScene->GetRegistry().emplace_or_replace<T>(
		        mHandle, std::forward<Args>(args)...);
		return c;
	}

	template<typename T>
	T& GetComponent()
	{
		ASSERT(HasComponent<T>(), "Component does not exist");
		return mScene->GetRegistry().get<T>(mHandle);
	}

	template<typename T>
	const T& GetComponent() const
	{
		ASSERT(HasComponent<T>(), "Component does not exist");
		return mScene->GetRegistry().get<T>(mHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		ASSERT(HasComponent<T>(), "Component does not exist");
		mScene->GetRegistry().remove<T>(mHandle);
	}

	explicit operator HandleType() const;
	explicit operator U32() const;
	explicit operator bool() const;

	bool operator==(const Entity& rhs) const;
	bool operator!=(const Entity& rhs) const;

private:
	HandleType mHandle {entt::null};
	Scene*     mScene {nullptr};
};
