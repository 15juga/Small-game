#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::PlayerCollision(Player& player, StaticGameObject& wall)
{
	return player.getSphere().Intersects(wall.GetBox());
}

bool CollisionManager::PlayerCollision(Player& player, Item& items)
{
	return !player.getSphere().Intersects(items.getSphere());
}

bool CollisionManager::PlayerCollision(Player& player, AIEnemyTurret& turret)
{
	return player.getSphere().Intersects(turret.getSphere());
}

void CollisionManager::PlayerCollision(Player& player, vector<BoundingBox>& box)
{
	bool collided = false;
	for (int i = 0; i < box.size(); i++)
	{
		if (player.getSphere().Intersects(box[i]))
		{
			player.uppdateBoundSpherePos();
			player.BounceBack(box[i]);
			collided = true;
		}
	}
}

bool CollisionManager::PlayerCollision(Player& player1, Player& player2)
{
	if (player1.getSphere().Intersects(player2.getSphere()))
		return true;
	return false;
}

bool CollisionManager::ProjectilePlayerCollision(Projectile& projectile, Player& player)
{
	if (projectile.getSphere().Intersects(player.getSphere()))
		return false;
	return true;
}

bool CollisionManager::ProjectileWallCollision(Projectile& projectile, StaticGameObject& wall)
{
	if (projectile.getSphere().Intersects(wall.GetBox()))
		return true; //if a object intersects it should return true that it has intersected
	return false;
}

bool CollisionManager::ProjectileOutSideWallCollision(Projectile& projectile, vector<BoundingBox>& box)
{
	for (int i = 0; i < box.size(); i++)
	{
		if (projectile.getSphere().Intersects(box[i]))
			return true;
	}
	return false;
}

bool CollisionManager::StaticStaticCollision(StaticGameObject& goal, StaticGameObject& wall)
{
	if (goal.GetBox().Intersects(wall.GetBox()))
		return true;
	return false;
}

bool CollisionManager::collisionUppdate(Player& player, AIEnemyTurret& turret, Item& items, StaticGameObject& wall)
{
	player.uppdateBoundSpherePos();
	turret.uppdateBoundSpherePos();
	items.updateBoundSphere();
	return true;
}

bool CollisionManager::ObjectTurret(StaticGameObject& object, AIEnemyTurret& turret)
{
	if (object.GetBox().Intersects(turret.getSphere()))
		return true;
	return false;
}


