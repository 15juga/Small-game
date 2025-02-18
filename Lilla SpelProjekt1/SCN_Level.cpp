#include "SCN_Level.h"

Level::Level(SCN& nextScn, std::string lvlPath, MeshList& meshList) :
	nextScn(nextScn), lvlName(""), meshList(meshList),
	COURSE_SIZE(80), SPAWN_MAX_DISTANCE(40), timer(0), boostTimer(0), slowTimer(0), counter(0)
{

	std::ifstream infile;
	infile.open(lvlPath);

	if (!infile.is_open()) ErrorLogger::Log("Could not open map file or map path is blank");
	srand(time(0));
	std::string str;
	while (getline(infile, str))
	{
		if (lvlName == "") lvlName = str;
		std::string inputTemp;
		Vector4 vectorTemp;
		infile >> inputTemp >> vectorTemp.x >> vectorTemp.y >> vectorTemp.z >> vectorTemp.w;
		vectorTemp.w *= (XM_PI / 180);
		if (inputTemp == "spawn1")
		{
			this->isSpawnRandom = false;
			this->spawnCoord[0].x = vectorTemp.x;
			this->spawnCoord[0].y = vectorTemp.y;
			this->spawnCoord[0].z = vectorTemp.z;
		}
		if (inputTemp == "spawn2")
		{
			this->isSpawnRandom = false;
			this->spawnCoord[1].x = vectorTemp.x;
			this->spawnCoord[1].y = vectorTemp.y;
			this->spawnCoord[1].z = vectorTemp.z;
		}
		if (inputTemp == "goal")
		{
			this->isGoalRandom = false;
			this->goalCoord.x = vectorTemp.x;
			this->goalCoord.y = vectorTemp.y;
			this->goalCoord.z = vectorTemp.z;
		}
		if (inputTemp == "wall")
		{
			wallCoords.push_back(vectorTemp);
		}
		if (inputTemp == "turret")
		{
			turretCoords.push_back(vectorTemp);
		}
		if (inputTemp == "item")
		{
			itemCoords.push_back(vectorTemp);
		}
	}
	this->walls = new StaticGameObject[wallCoords.size()];
	//this->items = new Item[itemCoords.size()];
	this->turrets = new AIEnemyTurret[turretCoords.size()];

	infile.close();
}

Level::~Level()
{
	turrets = nullptr;
	delete[] turrets;

	for (int i = 0; i < items.size(); i++)
	{
		items[i] = nullptr;
		delete items[i];
	}
	items.clear(); //not sure but might be handy to have

	walls = nullptr;
	delete[] walls;

	delete player[0];
	delete player[1];

	if (audioEngine)
	{
		audioEngine->Suspend();
	}
	bgInstance.reset();
}

void Level::init(ID3D11Device*& device, ID3D11DeviceContext*& deviceContext, constantBuffer<MatrixVertex>& ConstantBuffer)
{
	for (int i = 0; i < 2; i++)
	{
		this->player[i] = new Player;
	}

	stage.initialize(meshList.getMesh(ASSET::SANDBOX));
	stage.CreateBoundingBox();

	//stage.removeBoundingBox(); // the stage dont need to check collisions
	for (int i = 0; i < wallCoords.size(); i++)
	{
		walls[i].initialize(meshList.getMesh(ASSET::WALL));
		//walls[i].getTransform().SetPos(wallCoords[i].x, wallCoords[i].y, wallCoords[i].z);
		walls[i].getTransform().SetAngle(0, wallCoords[i].w, 0);
		if (walls[i].getTransform().GetRotation().y == 1.57079637f)
		{
			walls[i].getTransform().SetPos(wallCoords[i].x, wallCoords[i].y - 0.19f, wallCoords[i].z);
		}
		else
			walls[i].getTransform().SetPos(wallCoords[i].x, wallCoords[i].y, wallCoords[i].z);
		walls[i].CreateBoundingBox();
		walls[i].UpdateBox();
		//walls[i].scaleBox(2.0f);
		if (wallCoords[i].w == 1.57079637f)
		{
			walls[i].RotateBox();
		}
	}
	goal.initialize(meshList.getMesh(ASSET::TELEPORTER));
	goal.CreateBoundingBox();
	goal.UpdateBox();
	//goal.CreateBoundingBox();
	for (int i = 0; i < turretCoords.size(); i++)
	{
		turrets[i].initialize(meshList.getMesh(ASSET::TURRET));
		turrets[i].getTransform().SetPos(turretCoords[i].x, turretCoords[i].y, turretCoords[i].z);
		turrets[i].getTransform().SetAngle(0, turretCoords[i].w, 0);
		//turrets[i].getTransform().SetPos(-1.0f, -1.8f, 0.0f );
	}
	for (int i = 0; i < itemCoords.size(); i++)
	{
		Item* pickup = new Item();
		pickup->initialize(meshList.getMesh(ASSET::CUBE));
		pickup->getTransform().SetPos(itemCoords[i].x, itemCoords[i].y, itemCoords[i].z);
		pickup->getTransform().SetScale(.3, .3, .3);
		pickup->updateBoundSphere();
		pickup->getTransform().Rotate(0, itemCoords[i].w, 0);
		items.push_back(pickup);

		pickup = nullptr;
		delete pickup;
	}
	player[0]->initialize(meshList.getMesh(ASSET::PLAYER1));
	//player[0]->getTransform().Rotate(0, XM_PIDIV2, 0);
	player[1]->initialize(meshList.getMesh(ASSET::PLAYER2));
	if(isSpawnRandom) randomizeSpawn();
	else
	{
		this->player[0]->getTransform().SetPos(spawnCoord[0]);
		this->player[1]->getTransform().SetPos(spawnCoord[1]);
	}
	for (int i = 0; i < 2; i++)
	{
		player[i]->getTransform().SetScale(0.5f, 0.5f, 0.5f);
		if(!this->isSpawnRandom)
			player[i]->SetSpawn(spawnCoord[i].x, spawnCoord[i].y, spawnCoord[i].z);
	}
	RandomCoinSpawn(10);
	//player[0]->getTransform().SetPos(9, 0, -20);
	//player[0]->getTransform().SetPos(8, 0, 1);

	stage.getTransform().SetPos(0, -1.6f, 0);
	stage.getTransform().SetScale(1.07f, 1.07f, 1.07f);
	//Audio initialization
	{
		AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
		eflags |= AudioEngine_Debug;
#endif
		audioEngine = make_unique<AudioEngine>(eflags);

		plingSFX = std::make_unique<SoundEffect>(audioEngine.get(), L"Audio/item.wav");
		backgroundMusic = std::make_unique<SoundEffect>(audioEngine.get(), L"Audio/calm.wav");

		bgInstance = backgroundMusic.get()->CreateInstance();
		bgInstance->Play(true);

	}
	this->device = device;
	dContext = deviceContext;
	this->cBuffer = &ConstantBuffer;

	buildOuterWalls();
}

void Level::draW()
{
	for (int i = 0; i < 2; i++)
		player[i]->pDraw();
	for (int i = 0; i < wallCoords.size(); i++)
		walls[i].pDraw();
	for (int i = 0; i < turretCoords.size(); i++)
		turrets[i].pDraw();
	for (int i = 0; i < items.size(); i++)
		items[i]->pDraw();
	for (int i = 0; i < projectiles.size(); i++)
		projectiles[i]->pDraw();
	for (int i = 0; i < coins.size(); i++)
		coins[i]->pDraw();

	stage.pDraw();
	goal.pDraw();

	//UI
	rtd->BeginDraw();
	for (auto& b : textUI.textData) {

		textUI.Draw(rtd, b.second);
	}
	rtd->EndDraw();

	//newMEshOb.pDraw();
}

void Level::update(float deltaTime)
{
	player[0]->Move(deltaTime);
	player[1]->MoveOtherPlayer(deltaTime);


	for (int i = 0; i < turretCoords.size(); i++)
		turretUpdate(turrets[i], deltaTime);

	for (int i = 0; i < coins.size(); i++)
	{
		coins[i]->getTransform().Rotate(0, 1.f * deltaTime, 0);
		coins[i]->UpdateBox();
	}

	//if (Input::KeyDown('4'))
	//	this->nextScn = SCN::GAMEOVER;;

	//OutputDebugStringA(("Speed: " + to_string(player[0]->getSpeed()) + "\n").c_str());

	UpdateUI(deltaTime);

	float rotSpd = 1 * deltaTime;
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->itemRotate(rotSpd, rotSpd, rotSpd);
	}
	uppdateCollisionOnObjects(deltaTime);
	if (!audioEngine->Update())
	{
		//TODO
	}
}

void Level::killState()
{
	/*for (int i = 0; turrets.size(); i++)
	{
		delete turrets[i];
		turrets.erase(turrets.begin() + i);
	}*/
	//delete device;
	//delete dContext;
	//delete cBuffer;

	coins.clear();
	scoreSystem.SetPlayerScores(0, 0);
	timer = 0;
}

void Level::turretUpdate(AIEnemyTurret& turret, float deltaTime)
{
	turret.countDown(deltaTime);

	for (int i = 0; i < 2; i++)
	{
		if (turret.update(player[i]))
		{
			Projectile* projectile = new Projectile();
			projectile->Initialize(meshList.getMesh(ASSET::PROJECTILE));
			projectile->getTransform().SetScale(0.5f, 0.5f, 0.5f);
			projectile->getTransform().SetAngle(0.f, turret.getTransform().GetRotation().y, 0.f);
			turret.setProjectilePosition(*projectile);
			projectile->getTransform().MoveRelative(0, 1.25f, 1.3f);

			//OutputDebugStringA(("Projectile pos: " +
			//	to_string(projectile->getTransform().GetPos().x) + " , " +
			//	to_string(projectile->getTransform().GetPos().y) + " , " +
			//	to_string(projectile->getTransform().GetPos().z) + " \n").c_str());
			projectile->SetShot(true);
			projectiles.push_back(projectile);

		}
		for (int j = 0; j < projectiles.size(); j++)
		{
			//OutputDebugStringA(("Nr of projectiles: " + to_string(projectiles.size()) + " \n").c_str());

			projectileUpdate(*projectiles[j], *player[i], walls[i], deltaTime);

			if (projectiles[j]->GetShot() == false)
			{
				delete projectiles[j];
				projectiles.erase(projectiles.begin() + j);
			}
		}
	}
}

void Level::RandomCoinSpawn(int index)
{
	Vector2 coinPos;

	for (int i = index; i > 0; i--)
	{
		coinPos.x = static_cast<float>(rand() % COURSE_SIZE - (COURSE_SIZE / 2));
		coinPos.y = static_cast<float>(rand() % COURSE_SIZE - (COURSE_SIZE / 2));

		StaticGameObject* Coin = new StaticGameObject();
		Coin->initialize(meshList.getMesh(ASSET::COIN));
		Coin->scaleBox(1.2f);
		Coin->CreateBoundingBox();
		Coin->getTransform().SetPos(coinPos.x, 0, coinPos.y);

		if (CoinIsInside(Coin))	{
			RandomCoinSpawn(i);
		}

		else if (!CoinIsInside(Coin) && coins.size() < index){
			coins.push_back(Coin);
		}

		Coin = nullptr;
		delete Coin;
	}
}

bool Level::CoinIsInside(StaticGameObject*& coin)
{
	//Inte spawna inuti väggar
	for (int wI = 0; wI < wallCoords.size(); wI++)
	{
		coin->UpdateBox();
		if (collisionManager.StaticStaticCollision(*coin, walls[wI]))
		{
			return true;
		}
	}

	//Inte spawna inuti turrets
	for (int i = 0; i < turretCoords.size(); i++)
	{
		turrets[i].uppdateBoundSpherePos();
		coin->UpdateBox();
		if (collisionManager.ObjectTurret(*coin, turrets[i]))
		{
			return true;
		}
	}

	return false;
}

void Level::UpdateUI(float deltaTime)
{
	wstringstream wss, Score1, Score2;

	//For Timer
	timer += deltaTime;
	wss << timer;
	textUI.TimeText("Time", wss.str().c_str(), timer);

	//Scores
	Score1 << scoreSystem.getPlayerScore();
	textUI.ChangePoints("Score1", Score1.str().c_str());

	Score2 << scoreSystem.getPlayer2Score();
	textUI.ChangePoints("Score2", Score2.str().c_str());

	wss.str(std::wstring()); //Clear Contents
	Score1.str(std::wstring()); //Clear Contents
	Score2.str(std::wstring()); //Clear Contents
}

void Level::projectileUpdate(Projectile& proj, Player& player, StaticGameObject& wall, float deltaTime)
{
	proj.FireInDirection(deltaTime);
	proj.uppdateBoundSpherePos();

	if (!collisionManager.ProjectilePlayerCollision(proj, player))
	{
		player.Slow();
		proj.SetShot(false);
	}
}

// this is really just general collision logic and what happens if something collides
// would be a lot better if the objects themselves handled this logic but they dont
bool Level::uppdateCollisionOnObjects(float deltaTime)
{
	for (int i = 0; i < wallCoords.size(); i++)
	{
		for (int p = 0; p < projectiles.size(); p++)
		{
			if (collisionManager.ProjectileWallCollision(*projectiles[p], walls[i]) || collisionManager.ProjectileOutSideWallCollision(*projectiles[p], outerWalls))
			{
				projectiles[p]->SetShot(false);
			}
		}
	}

	float tmpRadius = player[0]->getSphere().Radius;
	for (int i = 0; i < 2; i++)
		player[i]->getSphere().Radius = 0.58f;
	if (collisionManager.PlayerCollision(*player[0], *player[1]))
	{
		player[0]->BounceBack(player[1]->getSphere());
		player[1]->BounceBack(player[0]->getSphere());
	}
	for (int i = 0; i < 2; i++)
		player[i]->getSphere().Radius = tmpRadius;

	for (int pI = 0; pI < 2; pI++)
	{
		player[pI]->uppdateBoundSpherePos();
		for (int tI = 0; tI < wallCoords.size(); tI++) // turret
		{
			if (collisionManager.PlayerCollision(*player[pI], walls[tI]))
			{
				player[pI]->DebugPos();
				player[pI]->BounceBack(walls[tI].GetBox());
			}
		}

		goal.UpdateBox();

		// Logic to see wether a player have enough coins
		// and then if they have reached the exit
		if (scoreSystem.getPlayerScore() >= 5)
		{
			int i = 0;
			if (collisionManager.PlayerCollision(*player[i], goal) && this->nextScn == SCN::LEVEL1)
			{
				bgInstance.get()->Stop();

				this->nextScn = SCN::LEVEL2;
			}

			if (this->nextScn == SCN::LEVEL2)
			{
				counter += deltaTime;
				if (counter > 5.f && collisionManager.PlayerCollision(*player[i], goal))
				{
					bgInstance.get()->Stop();

					this->nextScn = SCN::GAMEOVER;
				}
			}
		}
		else if (scoreSystem.getPlayer2Score() >= 5)
		{
			int i = 1;
			if (collisionManager.PlayerCollision(*player[i], goal) && this->nextScn == SCN::LEVEL1)
			{
				bgInstance.get()->Stop();

				this->nextScn = SCN::LEVEL2;
			}

			if (this->nextScn == SCN::LEVEL2)
			{
				counter += deltaTime;
				if (counter > 5.f && collisionManager.PlayerCollision(*player[i], goal))
				{
					bgInstance.get()->Stop();

					this->nextScn = SCN::GAMEOVER;
				}
			}
		}
		

		// send in player and outer walls as references--operations are made in the function. This is superios to the loop right above
		collisionManager.PlayerCollision(*player[pI], outerWalls);

		for (int tI = 0; tI < turretCoords.size(); tI++) // turret
		{
			turrets[tI].uppdateBoundSpherePos();
			if (collisionManager.PlayerCollision(*player[pI], turrets[tI]))
			{
				player[pI]->DebugPos();
				player[pI]->BounceBack(turrets[tI].getSphere());
			}
		}
		for (int iI = 0; iI < items.size(); iI++) // item
		{
			if (!collisionManager.PlayerCollision(*player[pI], *items[iI]))
			{
#ifdef _DEBUG
				if (!plingSFX.get()->IsInUse())
				{
					plingSFX->Play();
				}
#endif
				//if (plingSFX.get()->IsInUse())	OutputDebugStringA("in use\n");
				//else OutputDebugStringA("not in use\n");
				boostTimer = 0;
				player[pI]->SpeedBoost();
				delete items[iI];
				items.erase(items.begin() + iI);
			}
		}
		for (int cI = 0; cI < coins.size(); cI++)
		{
			if (collisionManager.PlayerCollision(*player[pI], *coins[cI]))
			{
				if (pI == 0){
					scoreSystem.addPlayerScore(1);
					//player[pI]->setSpeed(player[pI]->getSpeed() + (float)scoreSystem.getPlayerScore() * .006);
				}
				else if (pI == 1)
				{
					scoreSystem.addPlayer2Score(1);
					//player[pI]->setSpeed(player[pI]->getSpeed() + (float)scoreSystem.getPlayer2Score() * .006);
				}

				delete coins[cI];
				coins.erase(coins.begin() + cI);
			}
		}
		if (player[pI]->getSpeed() > player[pI]->GetOriginalSpeed())
		{
			boostTimer += deltaTime;
			if (boostTimer > 1.f)
			{
				player[pI]->RevertSpeed();
				boostTimer = 0;
			}
		}
		if (player[pI]->getSpeed() < player[pI]->GetOriginalSpeed())
		{
			slowTimer += deltaTime;
			if (slowTimer > 0.5f)
			{
				player[pI]->RevertSpeed();
				slowTimer = 0;
			}
		}
	}


	return true;
}

void Level::randomizeSpawn()
{
	Vector2 spawnPos;
	Vector2 goalPos;
	spawnPos.x = static_cast<float>(rand() % COURSE_SIZE - (COURSE_SIZE / 2));
	spawnPos.y = static_cast<float>(rand() % COURSE_SIZE - (COURSE_SIZE / 2));
	goalPos.x = static_cast<float>(rand() % COURSE_SIZE - (COURSE_SIZE / 2));
	goalPos.y = static_cast<float>(rand() % COURSE_SIZE - (COURSE_SIZE / 2));

	Vector2 vDistance = goalPos - spawnPos;
	float fDistance = sqrt(vDistance.x * vDistance.x + vDistance.y * vDistance.y);
	if (fDistance < SPAWN_MAX_DISTANCE)
	{
		//OutputDebugStringA("Spawn distance is: ");
		//OutputDebugStringA(std::to_string(fDistance).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("Reroll, Distance too small\n");
		randomizeSpawn();
	}
	else
	{
		//OutputDebugStringA("Spawn distance is: ");
		//OutputDebugStringA(std::to_string(fDistance).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("Distance big enough\n");
		this->player[0]->getTransform().SetPos(spawnPos.x - 2, 0, spawnPos.y);
		this->player[1]->getTransform().SetPos(spawnPos.x + 2, 0, spawnPos.y);
		this->goal.getTransform().SetPos(goalPos.x, 0, goalPos.y);
		this->goal.UpdateBox();

		//Ifall goal eller player spawnar i väggarna
		for (int wI = 0; wI < wallCoords.size(); wI++)
		{
			if (collisionManager.StaticStaticCollision(this->goal, walls[wI]))
			{
				//OutputDebugStringA("Reroll, goal collided with wall");
				randomizeSpawn();
			}
			//walls[wI].UpdateBox();
			for (int pI = 0; pI < 2; pI++)
			{

				player[pI]->uppdateBoundSpherePos();
				if (collisionManager.PlayerCollision(*this->player[pI], walls[wI]))
				{
					randomizeSpawn();
				}
			}
		}

		//Ifall player och goal spawnar i turrets
		//for (int tI = 0; tI < turretCoords.size(); tI++)
		//{
		//	for (int pI = 0; pI < 2; pI++)
		//	{
		//		player[pI]->uppdateBoundSpherePos();
		//		if (collisionManager.PlayerCollision(*player[pI], turrets[tI]))
		//		{
		//			randomizeSpawn();
		//		}

		//		if (collisionManager.ObjectTurret(goal, turrets[tI]))
		//		{
		//			randomizeSpawn();
		//		}
		//	}
		//}
	}	
}

void Level::buildOuterWalls()
{
	const int count = stage.GetBox().CORNER_COUNT;
	Vector3* corners = new Vector3[count];
	stage.GetBox().GetCorners(corners);


	BoundingBox box;

	for (int i = 0; i < count; i++)
	{
		Vector3 tmpVector = corners[i];
		tmpVector = Vector3::Zero;
		//we can use the corners to create a bounding box 
		//but we have this weird outcropping on the stage
		//which would push the walls bounding box too far away to be useful
	}

	box.Center = Vector3(44.5f,0,0);//right
	box.Extents = Vector3(1, 1000, 1000);
	outerWalls.push_back(box);

	box.Center = Vector3(-44.5f,0,0);// left
	box.Extents = Vector3(1, 1000, 1000);
	outerWalls.push_back(box);

	box.Center = Vector3(0,0,44.5f);// top
	box.Extents = Vector3(1000, 1000, 1);
	outerWalls.push_back(box);

	box.Center = Vector3(0,0,-44.5f);// bottom
	box.Extents = Vector3(1000, 1000, 1);
	outerWalls.push_back(box);

}

void Level::SetCamPos(CameraClass& camera, CameraClass& OCamera)
{
	camera.SetPos(this->player[0]->getTransform().GetPos().x, 8.f, this->player[0]->getTransform().GetPos().z + 3.f);
	OCamera.SetPos(this->player[1]->getTransform().GetPos().x, 8.f, this->player[1]->getTransform().GetPos().z + 3.f);
}

void Level::InitializeUI(HWND hwnd, int width, int height, ID2D1RenderTarget*& RTD)
{
	this->rtd = RTD;

	textUI.Initialize(hwnd, width, height, RTD);
	textUI.AddText(rtd, L"", L"Impact", 50.f, "Time", width / 2.f, 25.f, 500.f, 100.f, D2D1::ColorF::WhiteSmoke);

	//Scores
	textUI.AddText(rtd, L"Coins: ", L"Verdana", 25.f, "Coins1", 50.f, 40.f, 500.f, 50.f, D2D1::ColorF::WhiteSmoke);
	textUI.AddText(rtd, L"Coins: ", L"Verdana", 25.f, "Coins2", width - 75.f, 40.f, 500.f, 50.f, D2D1::ColorF::WhiteSmoke);

	textUI.AddText(rtd, L"0", L"Verdana", 25.f, "Score1", 100.f, 40.f, 500.f, 50.f, D2D1::ColorF::WhiteSmoke);
	textUI.AddText(rtd, L"0", L"Verdana", 25.f, "Score2", width - 25.f, 40.f, 500.f, 50.f, D2D1::ColorF::WhiteSmoke);
}

void Level::SetTimer(float& time)
{
	timer = time;
}

float Level::GetTime()
{
	return this->timer;
}

ScoreSystem Level::GetScore()
{
	return this->scoreSystem;
}

void Level::SetScores(int score1, int score2) 
{
	scoreSystem.SetPlayerScores(score1, score2);
}

