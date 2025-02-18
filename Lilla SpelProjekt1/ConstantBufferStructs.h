#pragma once

#include "SimpleMath.h"
using namespace DirectX;
using namespace SimpleMath;


struct ViewProjection
{
	Matrix view;
	Matrix projection;
};

// a camera is used to substitute for a light
// so these matrices should be from a camera, or 2
struct LightMatrices
{
	Matrix lightView;
	Matrix lightProjection;
	Vector4 lPos;
	Vector4 eyePos; // camera position probably, might be the primary cameras location
};

struct vertexData
{
	Vector3 pos = { 0.0f, 0.0f, 0.0f };
	Vector2 uv = { 0 ,0 };
	Vector3 normals = { 0.0f, 0.0f, 0.0f };
	vertexData operator =(Vertex v) 
	{	
		vertexData ver;
		ver.pos = v.pos;
		ver.uv = v.uv;
		ver.normals = v.normals;
		return ver;
	};

};
