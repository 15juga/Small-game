#pragma once
#include "Player.h"
#include "Camera.h"
#include "GameObject.h"
#include "StaticGameObject.h"
#include "AIEnemyTurret.h"
#include "CollisionManager.h"
#include "scoreSystem.h"
#include <Audio.h>
#include "Meshlist.h"

enum class SCN { E_NULL, MENU, LEVEL1, LEVEL2, GAMEOVER, CREDITS, LEADERBOARD };

class Scene
{
private:
	
public:
	virtual void init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer) = 0;
	virtual void draW() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void killState() = 0;
};
