#pragma once
#include "gameNode.h"
#include "Map.h"
#include "vaganteStructEnum.h"
/*
!vaganteStructEnum.h에서 선언했으니 참조만 할 것!
enum ENEMYSTATE {
	ENEMYSTATE_IDLE,
	ENEMYSTATE_MOVING,
	ENEMYSTATE_ATTACKING,
	ENEMYSTATE_HIT,
	ENEMYSTATE_JUMPING,
	ENEMYSTATE_FALLING,
	ENEMYSTATE_DEAD
};

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
*/

class Player;
class UI;
class Enemy : public gameNode
{
protected:
	image* _image;														//이미지
	ENEMYSTATE _state;													//상태
	Player* _player;													//플레이어 정보
	UI* _ui;															//ui
	Map* _map;															//적 인식용 맵정보
	tagStatusEffect _statusEffect[5];									//상태이상
	tagStat _statistics;												//스탯
	int _currentFrameX, _currentFrameY;									//프레임
	RECT _rc;															//피격범위
	float _pointx, _pointy;												//좌표
	float _xspeed, _yspeed, _angle, _gravity;							//넉백용 x,y축 이동 속도, 각도, 중력
	int _money;															//몬스터 죽으면 나올 동전 갯수
	mapInfo _upL, _upM, _upR, _midL, _midM, _midR, _botL, _botM, _botR;	//현재좌표기준 9개 타일
	bool _isFindPlayer;													//플레이어를 발견한 상태인지

public:
	HRESULT init();
	virtual HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	virtual void move();			// 이동관련함수
	virtual void jump();			// 점프
	virtual void attack();			// 공격


	//공격 받았을 시 (데미지만)
	void getDamaged(int damage) { _statistics.hp -= damage; }
	//공격 받았을 시 (데미지&넉백)
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _angle = angle; _gravity = 0; }
	//상태이상
	void addStatusEffect(tagStatusEffect statuseffect);

	//설정자&접근자
	int getHP() { return _statistics.hp; }
	void setHP(int hp) { _statistics.hp = hp; }
	tagStat getStat() { return _statistics; }
	void setStat(tagStat stat) { _statistics = stat; }
	RECT getRect() { return _rc; }
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	int getMoney() { return _money; }
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }
	void setTileInfo(mapInfo ul, mapInfo um, mapInfo ur, mapInfo ml, mapInfo mm, mapInfo mr, mapInfo bl, mapInfo bm, mapInfo br) { _upL = ul; _upM = um; _upR = ur; _midL = ml; _midM = mm; _midR = mr; _botL = bl; _botM = bm; _botR = br; }
	void setMap(Map* map) { _map = map; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Enemy();
	~Enemy();
};

