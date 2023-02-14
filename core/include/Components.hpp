#pragma once

#include <Common.hpp>

#include <Vector2.hpp>


struct TagComponent
{
	TagComponent()                    = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const String& tag): Tag(tag) {}

	String Tag;
};

struct TransformComponent
{
	TransformComponent()                          = default;
	TransformComponent(const TransformComponent&) = default;

	Vec2  Position {0.0f, 0.0f};
	Vec2  Scale {1.0f, 1.0f};
	float Rotation {0.0f};
};
