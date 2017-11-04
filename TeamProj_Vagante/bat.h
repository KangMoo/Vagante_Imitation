#pragma once
#include "Enemy.h"
#include "UI.h"


enum BATSTATE {
	BATSTATE_SLEEP,
	BATSTATE_FLYING,
	BATSTATE_HIT
};
class bat : public Enemy
{
private:
	bool _isPlayerOnTarget;	//�÷��̾� Ž������
	bool _isOnTop;			//õ�忡 ��Ҵ��� ���� üũ
	BATSTATE _batstate;
	int _hittimer;
	float _timerForFrame;

	RECT _rcHit; // �浹���� ���� �� by YJW
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
	void getDamaged(int damage, float angle, float knockbackpower) { 
		_statistics.hp -= damage; 
		_xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; 
		_batstate = BATSTATE_HIT; _hittimer = TIMEMANAGER->getWorldTime(); 
		_ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE);
	}
	void frameUpdate();					// ������ ������Ʈ
	void render(POINT camera);
	void draw(POINT camera);

public:
	bat();
	~bat();
};

