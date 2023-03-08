#include <Entity.hpp>

#include <Components.hpp>

Entity::Entity(HandleType handle, Scene* scene): mHandle(handle), mScene(scene)
{
}

const String& Entity::GetName() const
{
	return GetComponent<TagComponent>().Tag;
}

Entity::operator HandleType() const
{
	return mHandle;
}

Entity::operator uword() const
{
	return uword(mHandle);
}

Entity::operator bool() const
{
	return mHandle != entt::null;
}

bool Entity::operator==(const Entity& rhs) const
{
	return mHandle == rhs.mHandle && mScene == rhs.mScene;
}

bool Entity::operator!=(const Entity& rhs) const
{
	return !operator==(rhs);
}
