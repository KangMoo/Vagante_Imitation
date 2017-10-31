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
	virtual void falling();			// 낙하 처리
	virtual void rectResize();		// 혹시 rect 사이즈 변경 필요시 여길 통해서

	virtual void move();			// 이동관련함수
	virtual void attack();			// 공격
	virtual void frameUpdate();	// 프레임 업데이트
	worm();
	~worm();
};

