#pragma once
#include "gameNode.h"
#include "Enemy.h"

class Player;
class Map;
class EnemyManager : public gameNode
{
private:
	vector<Enemy*> _vEnemy;
	vector<Enemy*>::iterator _viEnemy;

	Player* _player;
	Map* _map;

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void deadEnemyCheck();

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setMapAddressLink(Map* map) { _map = map; }



	EnemyManager();
	~EnemyManager();
};

