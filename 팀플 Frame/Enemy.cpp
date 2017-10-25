#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}


HRESULT Enemy::init()
{
	return S_OK;
}
HRESULT Enemy::init(POINT point)
{

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
	move();
	collisionCheck();
	jump();
	attack();
}
void Enemy::render()
{

}

//그릴 때	x좌표에 (WINSIZEX/2 - camera.x) 만큼
//			y좌표에 (WINSIZEY/2 - camera.y) 만큼 더해주기!!!!
void Enemy::render(POINT camera)
{
	draw(camera);
}
void Enemy::draw(POINT camera)
{

}
void Enemy::move()

{

}
void Enemy::jump()			
{

}
void Enemy::attack()		
{

}

void Enemy::collisionCheck()
{
	//플레이어 아래 타일 체크
	switch (_downtile.type)
	{
		//아무것도 없을경우
	case MAPTILE_NULL:
		break;
		//벽&땅일경우
	case MAPTILE_WALL: case MAPTILE_WALL2:
		for (int i = 0; i < _yspeed; i++)
		{
			RECT temp;
			//바닥에 착지 할 경우
			if (IntersectRect(&temp, &_rc, &_downtile.rc))
			{
				//추가
			}
		}
		break;
		//사다리일경우
	case MAPTILE_LADDER:
		break;
		//발판일경우
	case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
		break;
		//함정일경우 (맵에서 처리)
	case MAPTILE_TRAP_SPIKE:case MAPTILE_TRAP_ARROW:case MAPTILE_TRAP_ROCK:
		break;
	}

	/*
	switch (downtile.type)
	{
	case MAPTILE_NULL:
	break;
	case MAPTILE_WALL:
	break;
	case MAPTILE_WALL2:
	break;
	case MAPTILE_LADDER:
	break;
	case MAPTILE_GROUND_CAN_GO_DOWN_1:
	break;
	case MAPTILE_GROUND_CAN_GO_DOWN_2:
	break;
	case MAPTILE_TRAP_SPIKE:
	break;
	case MAPTILE_TRAP_ARROW:
	break;
	case MAPTILE_TRAP_ROCK:
	break;
	}
	*/
}