#pragma once
#include "Enemy.h"
#include "UI.h"

class goblin : public Enemy
{
	UI* _ui;

	image* idleImg, *moveImg, *attackImgLeft, *attackImgRight, *jumpImg, *hitImg, *deadImg;
	image*attackImg;
	RECT _findPlayerRect;
	RECT _FOV;

	bool _right;

	bool _attack, _jump;

	float _findRange, _findRangeMax;
	float _saveY, _jumptimer;

	bool _isOnLand;

public:

	HRESULT init(POINT point, float minCog, float maxCog);
	void release();
	void update();
	void render(POINT camera);
	void draw(POINT camera);

	void move();
	void jump();
	void attack();
	void falling();

	void frameUpdate();
	void playerCog();


	void mapCollisionCheck();
	void getDamaged(int damage, float angle, float knockbackpower) 
	{ 
		_statistics.hp -= damage; 
		_xspeed += cosf(angle)*knockbackpower; 
		_yspeed -= sinf(angle)*knockbackpower;

		int t1 = _pointx;
		int t2 = _pointy;
		int t3 = damage;
		_ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE); 
		int test = 0;
	
	}



	goblin();
	~goblin();
};

