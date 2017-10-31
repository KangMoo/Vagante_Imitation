#pragma once
#include "gameNode.h"
#include "Map.h"
#include "vaganteStructEnum.h"
#include "FireBall.h"
/*
!vaganteStructEnum.h���� ���������� ������ �� ��!
enum BossSTATE {
BossSTATE_IDLE,
BossSTATE_MOVING,
BossSTATE_ATTACKING,
BossSTATE_HIT,
BossSTATE_JUMPING,
BossSTATE_FALLING,
BossSTATE_DEAD
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
class Boss : public gameNode
{
private:
	image* _image;														//�̹���
	ENEMYSTATE _state;													//����
	Player* _player;													//�÷��̾� ����
	UI* _ui;															//ui
	tagStatusEffect _statusEffect[5];									//�����̻�
	tagStat _statistics;												//����
	int _currentFrameX, _currentFrameY;									//������
	RECT _rc;															//�ǰݹ���
	float _pointx, _pointy;												//��ǥ
	float _xspeed, _yspeed;												//x,y�� �̵� �ӵ�
	int _money;															//���� ������ ���� ���� ����
	Map* _map;															//�� ����
	bool _isFindPlayer;													//�÷��̾ �߰��� ��������
	FireBall* _fireball;

	int _frameTime, _frameFPS;											//������ ��ȭ��
	float _minCog, _maxCog;												//���� ���� �νĹ���, �Ѱ� �νĹ���

	RECT _attackRect;

	int test1;
	int test2;
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
	virtual void frameUpdate();		// ������ ������Ʈ


									//���� �޾��� �� (��������)
	void getDamaged(int damage) { _statistics.hp -= damage; }
	//���� �޾��� �� (������&�˹�)
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; }
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

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUiAddressLink(UI* ui) { _ui = ui; }
	void setMapAddressLink(Map* map) { _map = map; }


	virtual void attRectClear() { _attackRect = RectMake(_pointx, _pointy, 1, 1); }
	virtual void statusEffect();

	Boss();
	~Boss();
};

