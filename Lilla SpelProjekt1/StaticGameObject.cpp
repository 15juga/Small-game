#include "StaticGameObject.h"


StaticGameObject::StaticGameObject()
{
}

StaticGameObject::~StaticGameObject()
{
}

bool StaticGameObject::CreateBoundingBox()
{
	box.CreateFromPoints(this->box, getMesh().getCount(), getMesh().getpPos(), 12);
	//box.CreateFromPoints(this->box, Vector3(-0.281247f, -0.665765f, -2.883065f), Vector3(0.281247f, 0.665765f, 2.883065f));
	return true;
}

bool StaticGameObject::initialize(MeshT& mesh)
{
	GameObject::initialize(mesh);
	return true;
}

void StaticGameObject::setSpawn(float x, float y, float z)
{
	transform.SetPos(x,y,z);
}

BoundingBox StaticGameObject::GetBox()
{
	return this->box;
}

void StaticGameObject::removeBoundingBox()
{
	this->box.Center = Vector3(100000, 100000, 100000);
	this->box.Extents = Vector3::Zero;
}

void StaticGameObject::UpdateBox()
{
	this->box.Center = transform.GetPos();
}

void StaticGameObject::scaleBox(float factor)
{
	this->box.Extents.x *= factor;
	this->box.Extents.y *= factor;
	this->box.Extents.z *= factor;
}

void StaticGameObject::RotateBox()
{
	float temp = this->box.Extents.x;
	this->box.Extents.x = this->box.Extents.z;
	this->box.Extents.z = temp;
}
