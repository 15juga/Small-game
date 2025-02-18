#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include "ShadowMap.h"
#include <DirectXMath.h>
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);

	DirectX::SimpleMath::Vector4 GetAmbientColor();
	DirectX::SimpleMath::Vector4 GetDiffuseColor();
	DirectX::SimpleMath::Vector3 GetPosition();

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);

	void GetViewMatrix(DirectX::SimpleMath::Matrix&);
	void GetProjectionMatrix(DirectX::SimpleMath::Matrix&);

private:
	DirectX::SimpleMath::Vector4 m_ambientColor;
	DirectX::SimpleMath::Vector4 m_diffuseColor;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Vector3 m_lookAt;
	DirectX::SimpleMath::Matrix m_viewMatrix;
	DirectX::SimpleMath::Matrix m_projectionMatrix;
};

#endif