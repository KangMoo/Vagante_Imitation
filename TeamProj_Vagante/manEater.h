#pragma once
#include "Enemy.h"

class manEater : public Enemy
{
private:

	image* _attacki, *_attackAfteri, *_deadi;

	int  _count;

	bool _attack;

	float _findRange,_findRangeMax;

	int _alpha;

public:
	HRESULT init(POINT point, float minCog, float maxCog);
	void release();
	void update();
	void render(POINT camera);
	void draw(POINT camera);

	void attack();			// ����
	void frameUpdate();	// ������ ������Ʈ

	manEater();
	~manEater();
};

