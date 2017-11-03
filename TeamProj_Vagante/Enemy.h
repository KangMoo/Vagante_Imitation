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
	float _minCog, _maxCog;												//���� ���� �νĹ���, �Ѱ� �νĹ���
	int _money;															//���� ������ ���� ���� ����
	mapInfo _upL, _upM, _upR, _midL, _midM, _midR, _botL, _botM, _botR;	//������ǥ���� 9�� Ÿ��
	bool _isFindPlayer;													//�÷��̾ �߰��� ��������
	int _frameTime, _frameFPS;											//������ ��ȭ��
	POINT _lastPlayerPoint;
	bool _dead;															//����üũ��
	int _deadAlpha;														//����üũ��
	RECT _attackRect;													//�÷��̾� ���ݿ� ��Ʈ
	bool _deleteForeEm;													//true�� em���� ����

public:
	virtual HRESULT init();
	virtual virtual HRESULT init(POINT point, float minCog, float maxCog);
	virtual void release();
	virtual void update();
	virtual void render();
	virtual void render(POINT camera);
	virtual void draw(POINT camera);

	virtual void move();			// �̵������Լ�
	virtual void jump();			// ����
	virtual void attack();			// ����
	virtual void frameUpdate() {}	// ������ ������Ʈ
	virtual void falling();			// ���� ó��
	virtual void rectResize();		// Ȥ�� rect ������ ���� �ʿ�� ���� ���ؼ�
	virtual void playerCog();		// �÷��̾�� ���� ������ �� �ִ��� �Ǵ�

	//���� �޾��� �� (��������)
	virtual void getDamaged(int damage) { _statistics.hp -= damage; }
	//���� �޾��� �� (������&�˹�)
	virtual void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _angle = angle; _gravity = 0; _state = ENEMYSTATE_HIT; }
	//�����̻�
	virtual void addStatusEffect(tagStatusEffect statuseffect);

	//������&������
	//ü��
	int getHP() { return _statistics.hp; }
	void setHP(int hp) { _statistics.hp = hp; }
	//���� ����
	tagStat getStat() { return _statistics; }
	//���� ���� ����
	void setStat(tagStat stat) { _statistics = stat; }
	//���� ��Ʈ
	RECT getRect() { return _rc; }
	//���� ���� ��ġ
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	//���� ���� ��ġ ����
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	//���Ͱ� ���� ��
	int getMoney() { return _money; }
	//���󰥶� �ӵ�
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }
	//���� ���� Ÿ�ϰ�
	void setTileInfo(mapInfo ul, mapInfo um, mapInfo ur, mapInfo ml, mapInfo mm, mapInfo mr, mapInfo bl, mapInfo bm, mapInfo br) { _upL = ul; _upM = um; _upR = ur; _midL = ml; _midM = mm; _midR = mr; _botL = bl; _botM = bm; _botR = br; }
	//�� ��ü�� ���� Ÿ�ϰ�
	void setMap(Map* map) { _map = map; }
	//�׾����� ��Ҵ���
	bool getdead() { return _dead; }
	//���� ����
	void setState(ENEMYSTATE es) { _state = es; }
	//���ݷ�Ʈ Ŭ����
	virtual void attRectClear() { _attackRect = RectMake(_pointx, _pointy, 1, 1); }
	//�����̻� ȿ�� ������
	virtual void statusEffect();

	//�÷��̾�, ui�� ����
	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUiAddressLink(UI* ui) { _ui = ui; }

	Enemy();
	~Enemy();
};

