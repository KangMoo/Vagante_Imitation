#pragma once
#include "Enemy.h"
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

	void isThereWall();

	worm();
	~worm();
};

