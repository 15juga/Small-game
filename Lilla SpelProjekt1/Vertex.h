#pragma once

#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;

struct Vertex
{
	Vector3 position;
	Vector2 uv;
	Vector3 normal;
};

struct ViewProjection
{
	Matrix view;
	Matrix projection;
};

// a camera is used to substitute for a light
struct LightMatrices 
{
	Matrix lightProjection;
	Matrix lightView;
	Vector4 lPos;
	Vector4 eyePos; // camera position probably, might be the primary cameras location
};