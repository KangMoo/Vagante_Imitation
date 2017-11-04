#pragma once
#include "Enemy.h"
#include "UI.h"

class worm :
	public Enemy
{
private:
	image* _moveLeft;
	image* _moveRight;
	image* _moveUp;
	image* _moveDown;
	image* _hit;

	bool _isLeft;
	int _whereIsWorm;
	float _moveSpeed;
	bool _reverseFrame;

public:
	//생성 위치, 최소인식범위, 최대인식범위
	HRESULT init(POINT point, float minCog, float maxCog);
	void falling();			// 낙하 처리
	void rectResize();		// 혹시 rect 사이즈 변경 필요시 여길 통해서
	void update();
	void render(POINT camera);
	void draw(POINT camera);
	void move();			// 이동관련함수
	void attack();			// 공격
	void frameUpdate();	// 프레임 업데이트
	void playerCog() {	_isFindPlayer = true;}	// 얜 플레이어 인식여부 관계없이 그냥 움직인다

	//데미지 입을때 지렁이는 떨어질때만 처리하므로 따로 처리합니다
	void getDamaged(int damage) { _statistics.hp -= damage; _ui->hitOutput(_pointx, _pointy, damage, LETTER_RED);}
	void getDamaged(int damage, float angle, float knockbackpower) 
	{ _statistics.hp -= damage; _xspeed += knockbackpower; _yspeed += knockbackpower; _angle = angle; _gravity = 0; _state = ENEMYSTATE_HIT; 
	_ui->hitOutput(_pointx, _pointy, damage, LETTER_RED);	}

	void isThereWall();									//가는 방향에 지형 변화가 있나요?
	void isDig();										//파묻혀있나요?

	worm();
	~worm();
};

