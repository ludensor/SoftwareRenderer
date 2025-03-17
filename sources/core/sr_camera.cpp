#include "sr_pch.h"
#include "core/sr_camera.h"

Camera::Camera(float fovy, float aspect, float near, float far)
	: fovy_(fovy)
	, aspect_(aspect)
	, near_(near)
	, far_(far)
{

}

math::Matrix4x4 Camera::GetViewMatrix() const
{
	return math::Matrix4x4();
}

math::Matrix4x4 Camera::GetProjectionMatrix() const
{
	return math::Matrix4x4();
}
