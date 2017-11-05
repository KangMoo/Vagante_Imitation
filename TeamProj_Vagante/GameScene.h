#pragma once
#include "gameNode.h"
#include "EnemyManager.h"
#include "Player.h"
#include "Map.h"
#include "UI.h"


class GameScene : public gameNode
{
private:
	EnemyManager* _em;
	Player* _player;
	Map* _map;
	UI* _ui;
	bool _UsingStatusWindow;
	POINT _camera;

	bool flagsound = false;

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void keyInput();
	void addImage();

	GameScene();
	~GameScene();
};

