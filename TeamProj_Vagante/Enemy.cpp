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
	//�ڽ�Ŭ�������� ���� �ʱ�ȭ�ϱ�
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
void Enemy::addStatusEffect(tagStatusEffect statuseffect)
{
	//�����̻� �߰�!
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL)
		{
			_statusEffect[i] = statuseffect;
			break;
		}
	}
}