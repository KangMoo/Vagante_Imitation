#pragma once
#include "Enemy.h"
#include "UI.h"
enum SLIMESTATE {
	SLIMESTATE_IDLE,
	SLIMESTATE_MOVING,
	SLIMESTATE_JUMPING,
	SLIMESTATE_FALLING,
	SLIMESTATE_DEAD
};

class slime : public Enemy
{
private:
	bool _isOnTop;			//õ�忡 ��Ҵ��� ���� üũ
	SLIMESTATE _slimeState;	//
	int _hittimer;			//
	float _jumptimer;		//���� Ÿ�̸�
	float _timerForFrame;
	bool _isOnLand;			//�� ���� �ִ��� ���� üũ
	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;			// 
public:
	//���� ��ġ, �ּ��νĹ���, �ִ��νĹ���
	HRESULT init(POINT point, float minCog, float maxCog);
	void update();				//	������Ʈ
	void render(POINT camera);
	void draw(POINT camera);
	void move();				// �̵������Լ�
	void actByState();			//
	void hitPlayer();			//�÷��̾� ���ݽ� ó��
	void mapCollisionCheck();	//�ʻ��� ��, �ٴ� �浹���� �Ǵ� �� ó��
	void deadcheck();
	void imgHandleByState();
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; _ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE); }
	void frameUpdate();					// ������ ������Ʈ
	
	slime();
	~slime();
};

