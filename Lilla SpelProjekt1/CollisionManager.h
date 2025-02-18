//responsible for checking on all the applicable GameObjects if collision has occured
#pragma once
#include "Player.h"
#include "GameObject.h"
#include "StaticGameObject.h"
#include "AIEnemyTurret.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3d11.h>
#include <SimpleMath.h>



using namespace DirectX;
using namespace SimpleMath;
class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	bool PlayerCollision(Player& player, StaticGameObject& wall);
	bool PlayerCollision(Player& player, Item& items);
	bool PlayerCollision(Player& player, AIEnemyTurret& turret);
	void PlayerCollision(Player& player, vector<BoundingBox> &box);
	bool PlayerCollision(Player& player1, Player& player2);

	bool ProjectilePlayerCollision(Projectile& projectile, Player& player);
	bool ProjectileWallCollision(Projectile& projectile, StaticGameObject& wall);
	bool ProjectileOutSideWallCollision(Projectile& projectile, vector<BoundingBox>& box);
	bool StaticStaticCollision(StaticGameObject& goal, StaticGameObject& wall);
	bool collisionUppdate(Player& player, AIEnemyTurret& turret, Item& items, StaticGameObject& wall);
	bool ObjectTurret(StaticGameObject& object, AIEnemyTurret& turret);
};


