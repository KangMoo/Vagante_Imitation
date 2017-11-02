#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

class EnemyManager;
class Player;
class UI;

/*
!vaganteStructEnum.h���� ���������� ������ �� ��!
enum MAPTILETYPE 
{
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

struct mapInfo 
{
	//Ÿ�� ����
	int type;
	//Ÿ�� ��ġ
	POINT point;
	//Ÿ�� ��Ʈ
	RECT rc;
};
*/

struct tagObj
{
	image* Image;
	RECT rc;
	float X, Y;
	int FrameX, FrameY;

	// ���� ���� ����
	bool _openBox;  

	// ���ڰ� ���Ⱦ����� Ȯ���ϱ� ���� �Ұ�
	bool _eventChk;
};


class Map : public gameNode
{
private:
	image* _trapImg;
	image* _mapImg;
	image* _BgImg;
	tagObj itemBox[2];
	tagObj coinBox[5];
	tagObj Door;
	




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
	void setObject();
	
	
	tagObj getCoinBox(int i) { return coinBox[i]; }// ���ιڽ��� ����
	tagObj getitemBox(int i) { return itemBox[i]; }// �����۹ڽ��� ����
	tagObj setCoinBox(int i, bool isOpen) { coinBox[i]._openBox = isOpen; }// ���ιڽ��� ���º�ȭ
	tagObj setItemBox(int i, bool isOpen) { itemBox[i]._openBox = isOpen; }// �����۹ڽ��� ���º�ȭ

	mapInfo getMapInfo(int i, int j) { return _mapInfo[i][j]; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Map();
	~Map();
};

