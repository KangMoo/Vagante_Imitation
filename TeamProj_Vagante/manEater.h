#pragma once
#include "Enemy.h"

class manEater :
	public Enemy
{
private:
	Enemy _enemy;

	RECT _attackRange;

	int  _count;

	bool _attack;

public:
	HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void attack();			// АјАн

	RECT getAttackRange() { return _attackRange; }

	manEater();
	~manEater();
};

