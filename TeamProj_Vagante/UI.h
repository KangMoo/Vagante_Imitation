#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

class EnemyManager;
class Player;
class Map;

/* 
!vaganteStructEnum.h에서 선언했으니 참조만 할 것!
struct tagStat {
	int hp;		//체력
	int str;	//힘
	int dex;	//민첩
	int vit;	//활력
	int inl;	//지능
	int lck;	//운
	int def;	//방어력
	int fir;	//불저항
	int ice;	//얼음저항
	int lgt;	//빛저항
	int psn;	//독저항
	int mel;	//근접공격데미지
	int rng;	//활공격데미지
	int crit;	//크리티컬확률
	int aspd;	//공속
	int spd;	//이속
};

enum ITEMSTATE {
	ITEMSTATE_ONMAP,
	ITEMSTATE_INVENTORY,
	ITEMSTATE_EQUIP
};
struct tagItemInfo {
	image* bigImg;			//아이템 이미지(큰거)
	image* smallImg;		//아이템 이미지(작은거)
	POINT point;			//아이템 좌표(맵에 있을 시)
	RECT rc;				//아이템 렉트(맵에 있을 시 사용)
	tagStat itemstat;		//아이템 능력치
	int itemstate;			//현재 아이템의 상태(맵,인벤토리,착용중인지 확인)
};
*/
struct tagCoinInfo {
	image* img;		//동전 이미지
	POINT point;	//동전 좌표(맵에 있을 시)
	RECT rc;		//동전 렉트(맵에 있을 시 사용)
};
class UI : public gameNode
{
private:
	bool _active;
	Player* _player;
	EnemyManager* _em;
	Map* _map;
	tagItemInfo _item[5];
	tagCoinInfo _coin[100];

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void draw();

	//맵에 아이템 출현~
	void addItemOnMap(tagItemInfo item);

	//맵에 돈 출현~ (위치정보만 가집니다~)
	void addCoinOnMap(POINT coinPoint); 

	//설정자 접근자
	bool getActive() { return _active; }
	void setActive(bool active) { _active = active; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }

	UI();
	~UI();
};

