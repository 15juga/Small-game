#pragma once
#include "ShadowMap.h"


using namespace DirectX;
using namespace SimpleMath;
class ShadowClass
{
private:

	struct MatrixBufferType
	{
		DirectX::SimpleMath::Matrix world;
		Matrix view;
		Matrix projection;
		Matrix lightView;
		Matrix lightProjection;
	};

	struct LightBufferType
	{
		Vector4 ambientColor;
		Vector4 diffuseColor;
	};

	struct LightBufferType2
	{
		Matrix lightPosition;
		float padding;
	};

public:
	ShadowClass();
	~ShadowClass();

	void BuildShadowMap(DirectX::SimpleMath::Vector3 dirLight, DirectX::SimpleMath::Matrix& ShadowTransform);
private:

};