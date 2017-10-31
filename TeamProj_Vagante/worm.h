#pragma once
#include "Enemy.h"
class worm :
	public Enemy
{
private:
	bool _isLeft;
	int _whereIsWorm;
	float _moveSpeed;
	bool _reverseFrame;

public:

	virtual HRESULT init(POINT point, float minCog, float maxCog);
	virtual void falling();			// ���� ó��
	virtual void rectResize();		// Ȥ�� rect ������ ���� �ʿ�� ���� ���ؼ�

	virtual void move();			// �̵������Լ�
	virtual void attack();			// ����
	virtual void frameUpdate();	// ������ ������Ʈ
	worm();
	~worm();
};

