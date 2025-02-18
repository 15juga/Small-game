#pragma once
#include "SceneClass.h"
#include "ButtonUI.h"
#include "TextUI.h"

class Level : public Scene
{
private:
	SCN& nextScn;
	MeshList& meshList;
	Player *player[2];
	std::string lvlName;
	//Item items;
	StaticGameObject stage;
	//AIEnemyTurret turret;

	CollisionManager collisionManager;
	ScoreSystem scoreSystem;
	/*StaticGameObject newMEshOb;*/

	Vector3 spawnCoord[2];
	bool isSpawnRandom = true;
	bool isGoalRandom = true;
	Vector3 goalCoord;

	vector<Vector4> wallCoords;
	//vector<StaticGameObject> walls;
	StaticGameObject* walls;

	vector<Vector4> turretCoords;
	AIEnemyTurret* turrets;

	vector<Vector4> projectileCoords;
	vector<Projectile*> projectiles;

	vector<Vector4> itemCoords;
	vector<Item*> items;
	//Item* items = new Item[40];

	StaticGameObject goal;
	vector<StaticGameObject*> coins; //5 coins example

	const int COURSE_SIZE;
	const float SPAWN_MAX_DISTANCE;

	unique_ptr<AudioEngine> audioEngine;
	unique_ptr<DirectX::SoundEffect> plingSFX;
	unique_ptr<DirectX::SoundEffectInstance> bgInstance;
	unique_ptr<DirectX::SoundEffect> backgroundMusic;
	float explodeDelay;

	ID3D11Device* device; 
	ID3D11DeviceContext* dContext;
	constantBuffer<MatrixVertex>* cBuffer;

	StaticGameObject newMEshOb;

	//create bounding volumes to represent the outer walls
	//
	vector<BoundingBox> outerWalls;

	//LEVEL UI
	TextUI textUI;
	ID2D1RenderTarget* rtd;
	float timer;
	float boostTimer;
	float slowTimer;
	float counter;

	void RandomCoinSpawn(int index);
	bool CoinIsInside(StaticGameObject*& coin);
	void UpdateUI(float deltaTime);

public:
	Level(SCN& nextScn, std::string lvlPath, MeshList& meshList);
	~Level();

	// Inherited via Scene
	virtual void init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer) override;
	virtual void draW() override;
	virtual void update(float deltaTime) override;
	virtual void killState() override;
	void turretUpdate(AIEnemyTurret& turret, float deltaTime);
	void projectileUpdate(Projectile& proj, Player& player, StaticGameObject& wall, float deltaTime);
	bool uppdateCollisionOnObjects(float deltaTime);
	void randomizeSpawn();

	void buildOuterWalls();

	void SetCamPos(CameraClass& camera, CameraClass& OCamera);
	void InitializeUI(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD);

	//Send data to next scene
	void SetTimer(float& time);
	float GetTime();

	ScoreSystem GetScore();
	void SetScores(int score1, int score2);
};