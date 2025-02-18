#include "Player.h"

Player::Player()
{
}
Player::~Player()
{

}

//bool Player::initialize(const char* filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer, const std::wstring texturePath)
//{
//	if (!GameObject::initialize(filePath, device, deviceContext, ConstantBuffer, texturePath))
//	{
//		ErrorLogger::Log("Failed to initialize in player class");
//		return false;
//	}
//	this->buildSphere();
//	this->hit = false;
//	this->speed = 0.2f;
//	return true;
//}

bool Player::initialize(MeshT& mesh)
{
	GameObject::initialize(mesh);
	this->speed = originalSpeed;
	return true;
}

//void Player::Update()
//{
//	Move();
//}
//
//void Player::Player2Update()
//{
//	MoveOtherPlayer();
//}

BoundingSphere &Player::getSphere()
{
	return this->sphere;
}

bool Player::buildSphere()
{
	//Change size of Radius
	//this->sphere.CreateFromPoints(this->sphere, getMesh().getnrV(), getMesh().getpPos(), 4);
	this->sphere.Center = transform.GetPos();
	this->sphere.Radius = (0.5f);
	return true;
}

void Player::DebugPos()
{
	//OutputDebugStringA(("Pos: X" + to_string(sphere.Center.x) + "	Y: " + to_string(sphere.Center.y) + "	Z: " + to_string(sphere.Center.z) + "\n").c_str());
	//OutputDebugStringA(("Pos: X" + to_string(axis.x) + "	Y: " + to_string(axis.y) + "	Z: " + to_string(axis.z) + "\n").c_str());
}

float Player::GetOriginalSpeed()
{
	return this->originalSpeed;
}

void Player::Slow()
{
	this->speed = 0.02f;
}
void Player::SpeedBoost()
{
	this->speed = speed * 1.5f;
}

void Player::RevertSpeed()
{
	this->speed = this->originalSpeed;
}

void Player::BounceBack(const BoundingBox& box)
{
	// this seems to work. though there might be some bugs
	// known bug is if there are two continers in a long row the player will stop moving at their intersection if he's gliding on on of them

	bool moved = false;
	Vector3 tmpPosition = this->initialPos;
	float xtmp = axis.x;

	//if (axis.x != 0 && axis.z != 0.0f) // use this for debugging diagonal movement
	//{
	//	OutputDebugStringA("a");
	//}

	// left & forward or backward
	if (axis.x > 0 && axis.z != 0.0f)
	{
		transform.SetPos(this->initialPos);
		axis.x = 0;
		transform.Move(axis.x * speed, 0, axis.z * speed);
		uppdateBoundSpherePos();

	}
	// right & forward or backward
	else if(axis.x < 0 && axis.z != 0.0f)
	{
		transform.SetPos(this->initialPos);
		axis.x = 0;
		transform.Move(axis.x * speed, 0, axis.z * speed);
		uppdateBoundSpherePos();
	}

	// if we are still intersecting we have hit a corner
	if (sphere.Intersects(box))
	{
		transform.SetPos(this->initialPos);
		uppdateBoundSpherePos();
	}
	else return; // if we havent hit a corner the rest of the code is useless

//--------------------------------
	// this code is superfluos and the program would work without it
	// but i leave the code so it is more clear what happens
	
	// if the player is at starting position
	if (tmpPosition == transform.GetPos())
		moved = false;
	else
		moved = true;

	// if we haven't moved 
	if (moved == false)
	{
//--------------------------------
		// reset the x movement since we set it to 0 earlier
		axis.x = xtmp;
		// if player moves backward and to a side
		if (axis.z > 0 && axis.x != 0.0f)
		{
			transform.SetPos(this->initialPos);
			axis.z = 0;
			transform.Move(axis.x * speed, 0, axis.z * speed);
			uppdateBoundSpherePos();
		}
		// if player moves forward and to a side
		else if (axis.z < 0 && axis.x != 0.0f)
		{
			transform.SetPos(this->initialPos);
			axis.z = 0;
			transform.Move(axis.x * speed, 0, axis.z * speed);
			uppdateBoundSpherePos();
		}
		else // if moving forward or backward into object
		{
			transform.SetPos(this->initialPos);
			uppdateBoundSpherePos();
		}

	}
	if (sphere.Intersects(box))
	{
		transform.SetPos(this->initialPos);
		uppdateBoundSpherePos();
	}
	else return;
}

void Player::BounceBack(const BoundingSphere& sphere2)
{
	bool moved = false;
	Vector3 tmpPosition = this->initialPos;
	float xtmp = axis.x;
	float ztmp = axis.z;
	float tmpRadius = this->sphere.Radius;


	// left || right && forward || backward
	if (axis.x != 0 && axis.z != 0.0f)
	{
		transform.SetPos(this->initialPos);
		axis.z = 0;
		transform.Move(axis.x * speed, 0, axis.z * speed);
		uppdateBoundSpherePos();

	}
	// if we are still intersecting we have hit a corner
	if (sphere.Intersects(sphere2))
	{
		transform.SetPos(this->initialPos);
		uppdateBoundSpherePos();
	}
	else return; // if we havent hit a corner the rest of the code is useless

//--------------------------------
	// this code is superfluous and the program would work without it
	// but i leave the code so it is more clear what happens

	// if the player is at starting position
	if (tmpPosition == transform.GetPos())
		moved = false;
	else
		moved = true;

	// if we haven't moved 
	if (moved == false)
	{
//--------------------------------
				// reset the z movement since we set it to 0 earlier
		axis.z = ztmp;
		// if player moves backward and to a side
		if (axis.z != 0 && axis.x != 0.0f)
		{
			transform.SetPos(this->initialPos);
			axis.x = 0;
			transform.Move(axis.x * speed, 0, axis.z * speed);
			uppdateBoundSpherePos();

		}

		else // if moving forward or backward into object
		{
			transform.SetPos(this->initialPos);
			uppdateBoundSpherePos();
		}
		if (sphere.Intersects(sphere2))
		{
			transform.SetPos(this->initialPos);
			uppdateBoundSpherePos();
		}
	}
}

void Player::SetColliding(bool t)
{
	colliding = t;
}

float Player::getSpeed() const
{
	return this->speed;
}

void Player::setSpeed(float speed)
{
	this->speed = speed;
	//this->originalSpeed = speed;
}

void Player::Move(float time)
{
	// create a temp position to move back to incase the object collides into another solid object
	initialPos = transform.GetPos();
	axis.x = Input::KeyPress('A') - Input::KeyPress('D');
	axis.z = Input::KeyPress('S') - Input::KeyPress('W');
	//axis.y = Input::KeyPress('E') - Input::KeyPress('Q');

	static Vector3 rot;
	if (axis.x != 0)
	{
		 rot = { transform.GetRotation().x , axis.x * -1.5708f, transform.GetRotation().z };
	}
	if (axis.z > 0)
	{
		 rot = { transform.GetRotation().x , (axis.z * -XM_PI) + (float)(axis.x * 0.785398), transform.GetRotation().z };
	}
	 if (axis.z < 0)
	{
		 rot = { transform.GetRotation().x , 0.0f + (float)(axis.x * -0.785398), transform.GetRotation().z };
	}


	transform.SetAngle(rot);
	axis.Normalize();
	transform.Move(Vector3(axis.x * speed * time, 0.f, axis.z * speed * time) * 100);
	uppdateBoundSpherePos();
}

void Player::MoveOtherPlayer(float time)
{
	// create a temp position to move back to incase the object collides into another solid object
	initialPos = transform.GetPos();
	axis.x = Input::KeyPress(VK_LEFT) - Input::KeyPress(VK_RIGHT);
	axis.z = Input::KeyPress(VK_DOWN) - Input::KeyPress(VK_UP);
	//axis.y = Input::KeyPress('E') - Input::KeyPress('Q');

	static Vector3 rot;
	if (axis.x != 0)
	{
		rot = { transform.GetRotation().x , axis.x * -1.5708f, transform.GetRotation().z };
	}
	if (axis.z > 0)
	{
		rot = { transform.GetRotation().x , (axis.z * -XM_PI) + (float)(axis.x * 0.785398), transform.GetRotation().z };
	}
	if (axis.z < 0)
	{
		rot = { transform.GetRotation().x , 0.0f + (float)(axis.x * -0.785398), transform.GetRotation().z };
	}


	transform.SetAngle(rot);
	axis.Normalize();
	transform.Move(Vector3(axis.x * speed * time, 0.f, axis.z * speed * time) * 100);
	uppdateBoundSpherePos();
}
bool Player::HitPlayer(Item*& items)
{
	return false;
}

void Player::SetSpawn(float x, float y, float z)
{
	transform.SetPos(x,y,z);
}

bool Player::uppdateBoundSpherePos()
{
	//Uppdating center of Radius
	this->sphere.Center = this->transform.GetPos();
	//OutputDebugStringA(("Pos: X" + to_string(transform.GetPos().x) + "	Y: " + to_string(transform.GetPos().y) + "	Z: " + to_string(transform.GetPos().z) + "\n").c_str());
	//axis = Vector3::Zero;
	return true;
}


