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

//�׸� ��	x��ǥ�� (WINSIZEX/2 - camera.x) ��ŭ
//			y��ǥ�� (WINSIZEY/2 - camera.y) ��ŭ �����ֱ�!!!!
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
	//�÷��̾� �Ʒ� Ÿ�� üũ
	switch (_downtile.type)
	{
		//�ƹ��͵� �������
	case MAPTILE_NULL:
		break;
		//��&���ϰ��
	case MAPTILE_WALL: case MAPTILE_WALL2:
		for (int i = 0; i < _yspeed; i++)
		{
			RECT temp;
			//�ٴڿ� ���� �� ���
			if (IntersectRect(&temp, &_rc, &_downtile.rc))
			{
				//�߰�
			}
		}
		break;
		//��ٸ��ϰ��
	case MAPTILE_LADDER:
		break;
		//�����ϰ��
	case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
		break;
		//�����ϰ�� (�ʿ��� ó��)
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