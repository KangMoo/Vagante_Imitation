#pragma once
#include "Enemy.h"
#include "vaganteStructEnum.h"

class maneater : public Enemy
{
private:
	Enemy _enemy;

	RECT _attackRange;

	int _delay,_count;

public:
	HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void attack();			// АјАн


	maneater();
	~maneater();
};

