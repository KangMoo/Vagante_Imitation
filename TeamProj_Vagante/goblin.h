#pragma once
#include "Enemy.h"
class goblin : public Enemy
{
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

	goblin();
	~goblin();
};

