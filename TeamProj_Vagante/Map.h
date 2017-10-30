#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

class EnemyManager;
class Player;
class UI;

/*
!vaganteStructEnum.h���� ���������� ������ �� ��!
enum MAPTILETYPE {
	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER,
	MAPTILE_GROUND_CAN_GO_DOWN_1,
	MAPTILE_GROUND_CAN_GO_DOWN_2,
	MAPTILE_TRAP_SPIKE,
	MAPTILE_TRAP_ARROW,
	MAPTILE_TRAP_ROCK
};

struct mapInfo {
	//Ÿ�� ����
	int type;
	//Ÿ�� ��ġ
	POINT point;
	//Ÿ�� ��Ʈ
	RECT rc;
};
*/
class Map : public gameNode
{
private:
	image* _mapImg;
	mapInfo _mapInfo[40][58];
	EnemyManager* _em;
	Player* _player;
	UI* _ui;

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);
	
	void setTile();

	mapInfo getMapInfo(int i, int j) { return _mapInfo[i][j]; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Map();
	~Map();
};

