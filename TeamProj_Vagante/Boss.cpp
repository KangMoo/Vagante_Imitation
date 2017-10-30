#include "stdafx.h"
#include "Boss.h"
#include "Player.h"
#include "UI.h"
#include "FireBall.h"

Boss::Boss()
{
}


Boss::~Boss()
{
}


HRESULT Boss::init()
{
	return S_OK;
}
HRESULT Boss::init(POINT point)
{
	//�ʱ�ȭ���ּ���
	_image = IMAGEMANAGER->findImage("test");
	_currentFrameX = 0;
	_currentFrameY = 0;									//������
	_pointx = point.x;
	_pointy = point.y;										//��ǥ
	_xspeed = _yspeed = 0;												//x,y�� �̵� �ӵ�
	_money = 10;															//���� ������ ���� ���� ����
	_isFindPlayer = false;													//�÷��̾ �߰��� ��������
	_fireball = new FireBall;
	_fireball->init(100, 1000, "����");


	
	return S_OK;
}
void Boss::release()
{

}
void Boss::update()
{
	_fireball->setMapAddressLink(_map);
	_fireball->setPlayerAddressLink(_player);
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
	//test~
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 5);
	}
	_fireball->update();

	//~test
}
void Boss::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

//�׸� ��	x��ǥ�� camera.x ��ŭ
//			y��ǥ�� camera.y ��ŭ �����ֱ�!!!!
void Boss::render(POINT camera)
{
	draw(camera);
}
void Boss::draw(POINT camera)
{
	_fireball->render(camera);
}
void Boss::move()

{

}
void Boss::jump()
{

}
void Boss::attack()
{

	//fireball �߻�
	//_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 6);

}
void Boss::addStatusEffect(tagStatusEffect statuseffect)
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