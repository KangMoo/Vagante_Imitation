 #pragma once
#include "Enemy.h"
class goblin : public Enemy
{
	bool _attack;
	bool _right; 

	int _count;

	float _findRange, _findRangeMax;

public:

	HRESULT init(POINT point, float minCog, float maxCog);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void move();		
	void jump();		
	void attack();
	void falling();

	goblin();
	~goblin();
};

