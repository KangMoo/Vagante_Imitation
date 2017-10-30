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

	_minCog = 150;
	_maxCog = 1500;

	
	return S_OK;
}
void Boss::release()
{

}
void Boss::update()
{
	attRectClear();
	statusEffect();

	_fireball->setMapAddressLink(_map);
	_fireball->setPlayerAddressLink(_player);
	if (_isFindPlayer)
	{
		_isFindPlayer = true;
		move();
		jump();
		attack();


		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > _maxCog)
			_isFindPlayer = false;
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//�����ӿ�ũ ������ ���Ͽ� _PlayerPoint�� _Player->getPoint()�� �����߽��ϴ�~~//
		///////////////////////////////////////////////////////////////////////////////////////

		//���� �νĻ����� ���Ϳ� �÷��̾��� �Ÿ��� �⺻ �νĹ��� ������ �� ���� ����
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ϳ� �÷��̾ ���� ���� �ִ�
				_isFindPlayer = true;
			}
			else
			{
				int count = 0;
				float x = 0;
				float y = 0;
				float dist = getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
				float angle = getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
				for (int i = 0; i < dist; i++)
				{
					float ox = (_pointx + i*cosf(angle)) / TILESIZE;
					float oy = (_pointy + i*-sinf(angle)) / TILESIZE;

					if (ox == x && oy == y) continue;

					x = ox;
					y = oy;

					if (static_cast<int>(_map->getMapInfo(y, x).type == 1))
						count++;
				}
				if (count >= 1) _isFindPlayer = false;
				else _isFindPlayer = true;
			}
		}
	}
	//test~
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 5);
	}
	_fireball->update();

	frameUpdate();
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
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
	if((_pointx > camera.x && _pointx < camera.x + WINSIZEX) &&
		(_pointy > camera.y && _pointy < camera.y + WINSIZEY))
		_image->frameRender(getMemDC(), _rc.left - camera.x, _rc.top - camera.y);
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

void Boss::statusEffect()
{
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL) continue;

		switch (_statusEffect[i].type)
		{
		case STATUSEFFECT_POISON:
			break;
		case STATUSEFFECT_FIRE:
			break;
		case STATUSEFFECT_STUN:
			break;
		case STATUSEFFECT_HEAL:
			break;
		}
	}
}

void Boss::frameUpdate()
{

}