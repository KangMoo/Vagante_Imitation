#pragma once
#include "gameNode.h"
#include "Map.h"
#include "vaganteStructEnum.h"
/*
!vaganteStructEnum.h���� ���������� ������ �� ��!
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
int hp;		//ü��
int str;	//��
int dex;	//��ø
int vit;	//Ȱ��
int inl;	//����
int lck;	//��
int def;	//����
int fir;	//������
int ice;	//��������
int lgt;	//������
int psn;	//������
int mel;	//�������ݵ�����
int rng;	//Ȱ���ݵ�����
int crit;	//ũ��Ƽ��Ȯ��
int aspd;	//����
int spd;	//�̼�
};
*/

class Player;
class UI;
class Enemy : public gameNode
{
protected:
	image* _image;														//�̹���
	ENEMYSTATE _state;													//����
	Player* _player;													//�÷��̾� ����
	UI* _ui;															//ui
	Map* _map;															//�� �νĿ� ������
	tagStatusEffect _statusEffect[5];									//�����̻�
	tagStat _statistics;												//����
	int _currentFrameX, _currentFrameY;									//������
	RECT _rc;															//�ǰݹ���
	float _pointx, _pointy;												//��ǥ
	float _xspeed, _yspeed, _angle, _gravity;							//�˹�� x,y�� �̵� �ӵ�, ����, �߷�
	int _money;															//���� ������ ���� ���� ����
	mapInfo _upL, _upM, _upR, _midL, _midM, _midR, _botL, _botM, _botR;	//������ǥ���� 9�� Ÿ��
	bool _isFindPlayer;													//�÷��̾ �߰��� ��������

public:
	HRESULT init();
	virtual HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	virtual void move();			// �̵������Լ�
	virtual void jump();			// ����
	virtual void attack();			// ����


	//���� �޾��� �� (��������)
	void getDamaged(int damage) { _statistics.hp -= damage; }
	//���� �޾��� �� (������&�˹�)
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _angle = angle; _gravity = 0; }
	//�����̻�
	void addStatusEffect(tagStatusEffect statuseffect);

	//������&������
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

