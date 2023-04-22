#include <Camera.hpp>

Camera::Camera(float left, float right, float bottom, float top)
        : mProjection(left, right, bottom, top),
          mAngle(0.0f),
          mZoom(1.0f),
          mCenterOffset((right - left) * 0.5f, (bottom - top) * 0.5f)
{
	Calculate();
}

void Camera::SetProjection(float left, float right, float bottom, float top)
{
	mProjection.Reset();
	mProjection.Project(left, right, bottom, top);
	mCenterOffset.x = (right - left) * 0.5f;
	mCenterOffset.y = (bottom - top) * 0.5f;
	mViewProjection = mProjection * mView;
	Calculate();
}

void Camera::SetCamera(vec2 position, float angle, float zoom, vec2 offset)
{
	Position = position;
	mAngle   = angle;
	mZoom    = zoom;
	mOffset  = offset;
	Calculate();
}

vec2 Camera::GetPosition() const
{
	return Position;
}

void Camera::SetPosition(vec2 position)
{
	Position = position;
	Calculate();
}

float Camera::GetRotation() const
{
	return mAngle;
}

void Camera::SetRotation(float angle)
{
	mAngle = angle;
	Calculate();
}

float Camera::GetZoom() const
{
	return mZoom;
}

void Camera::SetZoom(float zoom)
{
	mZoom = zoom;
	Calculate();
}

vec2 Camera::GetOffset() const
{
	return mOffset;
}

void Camera::SetOffset(vec2 offset)
{
	mOffset = offset;
	Calculate();
}

const Transform& Camera::GetView() const
{
	return mView;
}

const Transform& Camera::GetProjection() const
{
	return mProjection;
}

const Transform& Camera::GetViewProjection() const
{
	return mViewProjection;
}

void Camera::Calculate()
{
	mView.Reset();
	mView.Translate(mCenterOffset + mOffset)
	        .Scale(mZoom)
	        .Rotate(-mAngle)
	        .Translate(-Position);
	mViewProjection = mProjection * mView;
}
