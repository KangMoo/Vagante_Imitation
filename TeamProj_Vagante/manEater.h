#pragma once
#include "Enemy.h"

class manEater : public Enemy
{
private:

	int  _count;

	bool _attack;

	float _findRange,_findRangeMax;

public:
	HRESULT init(POINT point, float minCog, float maxCog);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void attack();			// АјАн

	manEater();
	~manEater();
};

