#pragma once

#include <DirectXMath.h>

class Camera {
public:
	Camera();
	~Camera() = default;

	void update(float dt);
	dx::XMFLOAT4X4 ViewProjection();
	dx::XMFLOAT4X4 CubeVP();
	dx::XMFLOAT3 Position() { return position; }
	dx::XMFLOAT3 Direction() { return direction; }
private:
	dx::XMFLOAT4X4 matrix;
	dx::XMFLOAT3 position;
	dx::XMFLOAT3 direction;
	float yaw = 0;
	float pitch = 0;
};