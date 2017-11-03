#pragma once
#include "Enemy.h"
enum BATSTATE {
	BATSTATE_SLEEP,
	BATSTATE_FLYING,
	BATSTATE_HIT
};
class bat : public Enemy
{
private:
	image* _image;			//�̹���
	bool _isdead;			//�������
	bool _isPlayerOnTarget;	//�÷��̾� Ž������
	bool _isOnTop;			//õ�忡 ��Ҵ��� ���� üũ
	RECT _rc;				//�ǰݷ�Ʈ
	float _alpha;
	BATSTATE _batstate;
	int _hittimer;
	float _pointx, _pointy;	//��ǥ
	float _timerForFrame;
public:
	//���� ��ġ, �ּ��νĹ���, �ִ��νĹ���
	HRESULT init(POINT point, float minCog, float maxCog);
	void update();				//	������Ʈ
	void move();				// �̵������Լ�
	void actByState();			//
	void hitPlayer();			//�÷��̾� ���ݽ� ó��
	void mapCollisionCheck();	//�ʻ��� ��, �ٴ� �浹���� �Ǵ� �� ó��
	void deadcheck();
	void imgHandleByState();
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _batstate = BATSTATE_HIT; _hittimer = TIMEMANAGER->getWorldTime(); }
	void frameUpdate();					// ������ ������Ʈ
	void render(POINT camera);
	void draw(POINT camera);

public:
	bat();
	~bat();
};

