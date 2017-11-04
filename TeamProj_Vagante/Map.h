#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"
class EnemyManager;
class Player;
class UI;

#define COINBOXMAX 5
#define ITEMBOXMAX 2

/*
!vaganteStructEnum.h에서 선언했으니 참조만 할 것!
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
	//타일 종류
	int type;
	//타일 위치
	POINT point;
	//타일 렉트
	RECT rc;
};
*/

struct tagObj
{
	image* Image;
	RECT rc;
	float X, Y;
	int FrameX, FrameY;

	// 상자 개봉 상태
	bool _openBox;  

	// 상자가 열렸었는지 확인하기 위한 불값
	bool _eventChk;
};
struct tagMiniMap
{
	image* Image;
	RECT playerRc;
	float X, Y;
};

struct tagSight
{
	image* Image;
	float X, Y;
	float alpha;
};
class Map : public gameNode
{
private:
	image* _trapImg;
	image* _mapImg;
	image* _BgImg;
	image* _SightImg;
	image* _SightBackImg;
	tagObj* itemBox[ITEMBOXMAX];
	tagObj* coinBox[COINBOXMAX];
	tagObj Door;
	tagMiniMap miniMap;



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
	
	
	tagObj* getCoinBox(int i) { return coinBox[i]; }// 코인박스의 정보
	tagObj* getitemBox(int i) { return itemBox[i]; }// 아이템박스의 정보
	void setCoinBox(int i, bool isOpen) { coinBox[i]->_openBox = isOpen; }// 코인박스의 상태변화
	void setItemBox(int i, bool isOpen) { itemBox[i]->_openBox = isOpen; }// 아이템박스의 상태변화

	mapInfo getMapInfo(int i, int j) { return _mapInfo[i][j]; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Map();
	~Map();
};

