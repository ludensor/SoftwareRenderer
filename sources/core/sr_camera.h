#pragma once

#include "core/sr_math.h"

class Camera
{
public:
	Camera(float fovy, float aspect, float near, float far);

	math::Matrix4x4 GetViewMatrix() const;
	math::Matrix4x4 GetProjectionMatrix() const;

private:
	math::Vector3 postion_;
	math::Vector3 target_;

	float fovy_;
	float aspect_;
	float near_;
	float far_;
};
