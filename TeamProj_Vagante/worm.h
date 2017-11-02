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

	bool _isLeft;
	int _whereIsWorm;
	float _moveSpeed;
	bool _reverseFrame;

public:

	HRESULT init(POINT point, float minCog, float maxCog);
	void falling();			// ���� ó��
	void rectResize();		// Ȥ�� rect ������ ���� �ʿ�� ���� ���ؼ�

	void move();			// �̵������Լ�
	void attack();			// ����
	void frameUpdate();	// ������ ������Ʈ
	void playerCog() {	_isFindPlayer = true;}	// �� �÷��̾� �νĿ��� ������� �׳� �����δ�

	void isThereWall();

	worm();
	~worm();
};

