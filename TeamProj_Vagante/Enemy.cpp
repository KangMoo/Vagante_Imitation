#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "UI.h"

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
	if (_isFindPlayer)
	{
		_isFindPlayer = true;
		move();
		jump();
		attack();

		_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());

		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > 150)
		_isFindPlayer = false;
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//�����ӿ�ũ ������ ���Ͽ� _PlayerPoint�� _Player->getPoint()�� �����߽��ϴ�~~//
		///////////////////////////////////////////////////////////////////////////////////////

		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 50)
		{
			if (_pointx < _player->getPoint().x && _pointy < _player->getPoint().y)
			{
				//���Ͱ� �÷��̾�� ���ʿ� �ְ� ���ʿ� �ִ�
			}
			else if (_pointx < _player->getPoint().x && _pointy > _player->getPoint().y)
			{
				//���Ͱ� �÷��̾�� ���ʿ� �ְ� �Ʒ��ʿ� �ִ�
			}
			else if (_pointx > _player->getPoint().x && _pointy < _player->getPoint().y)
			{
				//���Ͱ� �÷��̾�� �����ʿ� �ְ� ���ʿ� �ִ�
			}
			else if (_pointx > _player->getPoint().x && _pointy > _player->getPoint().y)
			{
				//���Ͱ� �÷��̾�� �����ʿ� �ְ� �Ʒ��ʿ� �ִ�
			}
			else
			{

			}
		}
	}
	
}
void Enemy::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

//�׸� ��	x��ǥ�� camera.x ��ŭ
//			y��ǥ�� camera.y ��ŭ �����ֱ�!!!!
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