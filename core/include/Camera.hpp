#pragma once

#include <Transform.hpp>
#include <Vector2.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(float left, float right, float bottom, float top);

	void SetProjection(float left, float right, float bottom, float top);

	void SetCamera(vec2 position, float angle, float zoom, vec2 offset);

	vec2 GetPosition() const;
	void SetPosition(vec2 position);

	float GetRotation() const;
	void  SetRotation(float angle);

	float GetZoom() const;
	void  SetZoom(float zoom);

	vec2 GetOffset() const;
	void SetOffset(vec2 offset);

	const Transform& GetView() const;
	const Transform& GetProjection() const;
	const Transform& GetViewProjection() const;

private:
	void Calculate();

	Transform mProjection;
	Transform mView;
	Transform mViewProjection;

	vec2  Position {0.0f};
	float mAngle {0.0f};
	float mZoom {1.0f};
	vec2  mCenterOffset {0.0f};
	vec2  mOffset {0.0f};
};
