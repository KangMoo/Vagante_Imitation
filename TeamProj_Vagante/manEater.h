#pragma once
#include "Enemy.h"
#include "UI.h"
class manEater : public Enemy
{
private:

	UI* _ui;

	image* _attacki, *_attackAfteri, *_deadi;
	
	RECT _findRectRange;

	int  _count;

	bool _attack;

	float _findRange;

public:
	HRESULT init(POINT point, float minCog, float maxCog);
	void release();
	void update();
	void render(POINT camera);
	void draw(POINT camera);

	void attack();			// 공격
	void frameUpdate();	// 프레임 업데이트

	void getDamaged(int damage, float angle, float knockbackpower)
	{
		_statistics.hp -= damage; _state = ENEMYSTATE_HIT; 	_ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE);
	}


	manEater();
	~manEater();
};

