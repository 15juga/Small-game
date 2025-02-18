#pragma once
#include <DirectXMath.h>
#include <SimpleMath.h>
using namespace DirectX;
using namespace SimpleMath;

struct MatrixVertex
{
	Matrix world;
};

struct CSPos
{
	Vector3 position;
};

struct WVPMatrix
{
	Matrix WVPmat;
};