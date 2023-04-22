#pragma once

#include <Camera.hpp>
#include <Color.hpp>
#include <Common.hpp>
#include <Texture.hpp>
#include <Vector2.hpp>

struct TagComponent
{
	TagComponent()                    = default;
	TagComponent(const TagComponent&) = default;
	explicit TagComponent(String tag)
	        : Tag(std::move(tag))
	{
	}

	String Tag;
};

struct TransformComponent
{
	TransformComponent()                          = default;
	TransformComponent(const TransformComponent&) = default;

	vec2  Position {0.0f};
	vec2  Scale {1.0f};
	float Rotation {0.0f};
};

struct SpriteRendererComponent
{
	SpriteRendererComponent() = default;
	explicit SpriteRendererComponent(TexturePtr& texture)
	        : Texture(texture)
	{
	}
	SpriteRendererComponent(const SpriteRendererComponent&) = default;

	TexturePtr Texture;
	Color      Color {Color::WHITE};
	bool       FlipX {false};
	bool       FlipY {false};
};

struct CircleRendererComponent
{
	CircleRendererComponent()                               = default;
	CircleRendererComponent(const CircleRendererComponent&) = default;

	Color Color {Color::WHITE};
	float Thickness {1.0f};
	float Smoothness {0.03f};
};

struct CameraComponent
{
	CameraComponent() = default;
	CameraComponent(float left, float right, float bottom, float top)
	        : Camera {left, right, bottom, top}
	{
	}
	CameraComponent(const CameraComponent&) = default;

	Camera Camera;
};
