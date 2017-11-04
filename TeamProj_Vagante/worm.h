#pragma once
#include "Enemy.h"
#include "UI.h"

class worm :
	public Enemy
{
private:
	image* _moveLeft;
	image* _moveRight;
	image* _moveUp;
	image* _moveDown;
	image* _hit;

	bool _isLeft;
	int _whereIsWorm;
	float _moveSpeed;
	bool _reverseFrame;

public:
	//���� ��ġ, �ּ��νĹ���, �ִ��νĹ���
	HRESULT init(POINT point, float minCog, float maxCog);
	void falling();			// ���� ó��
	void rectResize();		// Ȥ�� rect ������ ���� �ʿ�� ���� ���ؼ�
	void update();
	void render(POINT camera);
	void draw(POINT camera);
	void move();			// �̵������Լ�
	void attack();			// ����
	void frameUpdate();	// ������ ������Ʈ
	void playerCog() {	_isFindPlayer = true;}	// �� �÷��̾� �νĿ��� ������� �׳� �����δ�

	//������ ������ �����̴� ���������� ó���ϹǷ� ���� ó���մϴ�
	void getDamaged(int damage) { _statistics.hp -= damage; _ui->hitOutput(_pointx, _pointy, damage, LETTER_RED);}
	void getDamaged(int damage, float angle, float knockbackpower) 
	{ _statistics.hp -= damage; _xspeed += knockbackpower; _yspeed += knockbackpower; _angle = angle; _gravity = 0; _state = ENEMYSTATE_HIT; 
	_ui->hitOutput(_pointx, _pointy, damage, LETTER_RED);	}

	void isThereWall();									//���� ���⿡ ���� ��ȭ�� �ֳ���?
	void isDig();										//�Ĺ����ֳ���?

	worm();
	~worm();
};

