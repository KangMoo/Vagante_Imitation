#pragma once
#include "gameNode.h"
#include "Map.h"

enum ENEMYSTATE {
	ENEMYSTATE_IDLE,
	ENEMYSTATE_MOVING,
	ENEMYSTATE_ATTACKING,
	ENEMYSTATE_HIT,
	ENEMYSTATE_JUMPING,
	ENEMYSTATE_FALLING,
	ENEMYSTATE_DEAD
};
class Enemy : public gameNode
{
protected:
	image* _image;							//이미지
	ENEMYSTATE _state;						//상태
	int _hp;								//hp
	int _currentFrameX, _currentFrameY;		//프레임
	RECT _rc;								//피격범위
	float _pointx, _pointy;					//좌표
	float _speed;							//몬스터의 이동속도
	float _xspeed, _yspeed;					//x,y축 이동 속도
	POINT _playerPoint;						//플레이어의 위치
	mapInfo _uptile, _downtile, _lefttile, _righttile;

public:
	HRESULT init();
	virtual HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void move();			// 이동관련함수
	void collisionCheck();	// 지형충돌체크
	void jump();			// 점프
	void attack();			// 공격

	//설정자&접근자
	int getHP() { return _hp; }
	void setHP(int hp) { _hp = hp; }
	RECT getRect() { return _rc; }
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	void setPlayerPoint(POINT playerpoint) { _playerPoint = playerpoint; }
	void setTileInfo(mapInfo uptile, mapInfo downtile, mapInfo lefttile, mapInfo righttile) { _uptile = uptile; _downtile = downtile; _lefttile = lefttile; _righttile = righttile; }
	//넉백을 위한 x,yspeed 설정&접근자
	void getDamaged(int damage) { _hp -= damage; }
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }


	Enemy();
	~Enemy();
};

