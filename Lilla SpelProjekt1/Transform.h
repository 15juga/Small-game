#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class Transform
{
protected:
	Vector3 pos, rot, scale;

public:

	Transform();

	virtual void SetPos(const Vector3& pos);
	virtual void Move(const Vector3& newPos);
	virtual void MoveRelative(const Vector3& pos);
	virtual void SetAngle(const Vector3& angle);
	virtual void Rotate(const Vector3& rotation);
	virtual void SetScale(const Vector3& scale);

	virtual void SetPos(float x, float y, float z);
	virtual void Move(float x, float y, float z);
	virtual void MoveRelative(float x, float y, float z);
	virtual void SetAngle(float x, float y, float z);
	virtual void Rotate(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);

	const Vector3& GetPos()const;
	const Vector3& GetRotation()const;
	const Vector3& GetScale()const;
	const Matrix GetTransform()const;
};