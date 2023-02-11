#pragma once

#include <Transform.hpp>
#include <Vector2.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(float left, float right, float bottom, float top);

	void SetProjection(float left, float right, float bottom, float top);

	void SetCamera(Vec2 position, float angle, float zoom, Vec2 offset);

	Vec2 GetPosition() const;
	void SetPosition(Vec2 position);

	float GetRotation() const;
	void  SetRotation(float angle);

	float GetZoom() const;
	void  SetZoom(float zoom);

	Vec2 GetOffset() const;
	void SetOffset(Vec2 offset);

	const Transform& GetView() const;
	const Transform& GetProjection() const;
	const Transform& GetViewProjection() const;

private:
	void Calculate();

	Transform mProjection;
	Transform mView;
	Transform mViewProjection;

	Vec2  mPosition {0, 0};
	float mAngle {0};
	float mZoom {1};
	Vec2  mCenterOffset {0};
	Vec2  mOffset {0};
};
