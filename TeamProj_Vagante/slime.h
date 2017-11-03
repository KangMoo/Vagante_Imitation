#pragma once
#include "Enemy.h"
enum SLIMESTATE {
	SLIMESTATE_IDLE,
	SLIMESTATE_MOVING,
	SLIMESTATE_JUMPING,
	SLIMESTATE_FALLING,
	SLIMESTATE_DEAD
};

class slime : public Enemy
{
private:
	image* _image;			//이미지
	bool _isPlayerOnTarget;	//플레이어 탐지여부
	bool _isOnTop;			//천장에 닿았는지 여부 체크
	RECT _rc;				//피격렉트
	float _alpha;			//
	SLIMESTATE _slimeState;	//
	int _hittimer;			//
	float _jumptimer;		//점프 타이머
	float _pointx, _pointy;	//좌표
	float _timerForFrame;
	bool _isOnLand;			//땅 위에 있는지 여부 체크
	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;			// 
public:
	//생성 위치, 최소인식범위, 최대인식범위
	HRESULT init(POINT point, float minCog, float maxCog);
	void update();				//	업데이트
	void render(POINT camera);
	void draw(POINT camera);
	void move();				// 이동관련함수
	void actByState();			//
	void hitPlayer();			//플레이어 공격시 처리
	void mapCollisionCheck();	//맵상의 벽, 바닥 충돌여부 판단 후 처리
	void deadcheck();
	void imgHandleByState();
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower;}
	void frameUpdate();					// 프레임 업데이트
	
	slime();
	~slime();
};

