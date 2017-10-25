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
	//자식클래스에서 각자 초기화하기
	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
	move();
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
void Enemy::addStatusEffect(tagStatusEffect statuseffect)
{
	//상태이상 추가!
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL)
		{
			_statusEffect[i] = statuseffect;
			break;
		}
	}
}