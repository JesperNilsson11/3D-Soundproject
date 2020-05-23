#include "pch.h"
#include "camera.h"
#include "input.h"

Camera::Camera() {
	position.x = 0.f;
	position.y = 1.f;
	position.z = -12.f;
}

void Camera::update(float dt) {
	pitch += Input::YmouseDelta() * dt;
	yaw += Input::XmouseDelta() * dt;

	if (pitch > dx::XM_PIDIV2) {
		pitch = dx::XM_PIDIV2;
	}
	if (pitch < -dx::XM_PIDIV2) {
		pitch = -dx::XM_PIDIV2;
	}

	dx::XMMATRIX x = dx::XMMatrixRotationX(pitch);
	dx::XMMATRIX y = dx::XMMatrixRotationY(yaw);

	dx::XMVECTOR right = dx::XMVector3Transform({ 1,0,0 }, y);
	dx::XMVECTOR dir = dx::XMVector3Transform({ 0,0,1 }, y);
	dx::XMStoreFloat3(&direction, dir);

	float speedup = 1.f;
	if (Input::KeyDown('E')) {
		speedup = 2.8f;
	}

	dx::XMVECTOR pos = dx::XMLoadFloat3(&position);
	if (Input::KeyDown('W')) {
		pos = dx::XMVectorAdd(pos, dx::XMVectorScale(dir, 7.1 * dt * speedup));
	}
	if (Input::KeyDown('S')) {
		pos = dx::XMVectorAdd(pos, dx::XMVectorScale(dir, -7.1 * dt*speedup));
	}
	if (Input::KeyDown('A')) {
		pos = dx::XMVectorAdd(pos, dx::XMVectorScale(right, -7.1 * dt));
	}
	if (Input::KeyDown('D')) {
		pos = dx::XMVectorAdd(pos, dx::XMVectorScale(right, 7.1 * dt));
	}

	dx::XMMATRIX rot = dx::XMMatrixMultiply(x, y);
	dx::XMVECTOR up = dx::XMVector3Transform({ 0,1,0 }, rot);
	dir = dx::XMVector3Transform({ 0,0,1 }, rot);
	dx::XMStoreFloat3(&position, pos);
	dx::XMMATRIX v = dx::XMMatrixLookToLH(pos, dir, up);
	dx::XMMATRIX p = dx::XMMatrixPerspectiveFovLH(3.14 / 6.0, 16.0 / 9.0, 0.1, 200.0);

	p = dx::XMMatrixMultiply(v, p);
	dx::XMStoreFloat4x4(&matrix, p);
}

dx::XMFLOAT4X4 Camera::ViewProjection() {
	return matrix;
}

dx::XMFLOAT4X4 Camera::CubeVP()
{
	dx::XMMATRIX vp = dx::XMLoadFloat4x4(&matrix);
	vp = dx::XMMatrixMultiply(dx::XMMatrixTranslation(position.x, position.y, position.z), vp);
	dx::XMFLOAT4X4 mat;
	dx::XMStoreFloat4x4(&mat, vp);
	return mat;
}
