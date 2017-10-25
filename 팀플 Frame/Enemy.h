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
	image* _image;							//�̹���
	ENEMYSTATE _state;						//����
	int _hp;								//hp
	int _currentFrameX, _currentFrameY;		//������
	RECT _rc;								//�ǰݹ���
	float _pointx, _pointy;					//��ǥ
	float _speed;							//������ �̵��ӵ�
	float _xspeed, _yspeed;					//x,y�� �̵� �ӵ�
	POINT _playerPoint;						//�÷��̾��� ��ġ
	mapInfo _uptile, _downtile, _lefttile, _righttile;

public:
	HRESULT init();
	virtual HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void move();			// �̵������Լ�
	void collisionCheck();	// �����浹üũ
	void jump();			// ����
	void attack();			// ����

	//������&������
	int getHP() { return _hp; }
	void setHP(int hp) { _hp = hp; }
	RECT getRect() { return _rc; }
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	void setPlayerPoint(POINT playerpoint) { _playerPoint = playerpoint; }
	void setTileInfo(mapInfo uptile, mapInfo downtile, mapInfo lefttile, mapInfo righttile) { _uptile = uptile; _downtile = downtile; _lefttile = lefttile; _righttile = righttile; }
	//�˹��� ���� x,yspeed ����&������
	void getDamaged(int damage) { _hp -= damage; }
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }


	Enemy();
	~Enemy();
};

