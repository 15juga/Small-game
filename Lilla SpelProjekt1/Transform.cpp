#include "Transform.h"

Transform::Transform() :scale(1, 1, 1)
{

}

void Transform::SetPos(const Vector3& pos)
{
	this->pos = pos;
}

void Transform::Move(const Vector3& newPos)
{
	this->pos += newPos;
}

void Transform::MoveRelative(const Vector3& pos)
{
	this->pos += Vector3::Transform(pos, XMMatrixRotationRollPitchYawFromVector(this->rot));
}

void Transform::SetAngle(const Vector3& angle)
{
	this->rot = angle;
}

void Transform::Rotate(const Vector3& rotation)
{
	this->rot += rotation;
}

void Transform::SetScale(const Vector3& scale)
{
	this->scale = scale;
}

void Transform::SetPos(float x, float y, float z)
{
	this->pos = Vector3(x, y, z);
}

void Transform::Move(float x, float y, float z)
{
	this->pos += Vector3(x, y, z);
}

void Transform::MoveRelative(float x, float y, float z)
{
	this->pos += Vector3::Transform(Vector3(x, y, z), XMMatrixRotationRollPitchYawFromVector(this->rot));
}

void Transform::SetAngle(float x, float y, float z)
{
	this->rot = Vector3(x, y, z);
}

void Transform::Rotate(float x, float y, float z)
{
	this->rot += Vector3(x, y, z);
}

void Transform::SetScale(float x, float y, float z)
{
	this->scale = Vector3(x, y, z);
}

const Vector3& Transform::GetPos() const
{
	return this->pos;
}

const Vector3& Transform::GetRotation() const
{
	return this->rot;
}

const Vector3& Transform::GetScale() const
{
	return this->scale;
}

const Matrix Transform::GetTransform() const
{
	Matrix transformation = XMMatrixScalingFromVector(scale) * XMMatrixRotationRollPitchYawFromVector(rot) * XMMatrixTranslationFromVector(pos);
	return transformation;
}
