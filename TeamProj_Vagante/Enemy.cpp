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

	//�˹�ó��, x���� 0�� �ƴҰ��̶� �����ϰ� ����
	if (_xspeed != 0)
	{
		_state = ENEMYSTATE_FALLING;
		_pointx -= cosf(_angle) * _xspeed;
		_gravity -= 0.4f;
		if (_gravity > 10) _gravity = 10;
		_yspeed += _gravity;
		_pointy += -sinf(_angle) * _yspeed;

		if (static_cast<int>(_pointy + _yspeed) % TILESIZE > 16)
		{
			if (static_cast<int>(_pointx - cosf(_angle)*_xspeed) == static_cast<int>(_pointx))
			{
				if (_botM.type == 1)
				{
					_state = ENEMYSTATE_IDLE;
					_pointx -= cosf(_angle) * _xspeed;
					_xspeed = 0;
					_yspeed = 0;
					_gravity = 0;
					_angle = 0;

					_pointy = _botM.rc.top - _image->getFrameHeight() / 2;
				}
			}
			else
			{
				if (cosf(_angle)*_xspeed > 0)
				{
					if (_botR.type == 1)
					{
						_state = ENEMYSTATE_IDLE;
						_pointx -= cosf(_angle) * _xspeed;
						_xspeed = 0;
						_yspeed = 0;
						_gravity = 0;
						_angle = 0;

						_pointy = _botM.rc.top - _image->getFrameHeight() / 2;
					}
				}
				else
				{
					if (_botL.type == 1)
					{
						_state = ENEMYSTATE_IDLE;
						_pointx -= cosf(_angle) * _xspeed;
						_xspeed = 0;
						_yspeed = 0;
						_gravity = 0;
						_angle = 0;

						_pointy = _botM.rc.top - _image->getFrameHeight() / 2;
					}
				}
			}
		}
	}

	if (_isFindPlayer)
	{
		_isFindPlayer = true;
		move();
		jump();
		attack();

		_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());

		//���� �� ������ �Ÿ��� 250 �̻����� �������� �Ѵ°� �����Ѵ�
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > 250)
			_isFindPlayer = false;
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//�����ӿ�ũ ������ ���Ͽ� _PlayerPoint�� _Player->getPoint()�� �����߽��ϴ�~~//
		///////////////////////////////////////////////////////////////////////////////////////

		//���� �νĻ����� ���Ϳ� �÷��̾��� �Ÿ��� 150 ������ �� ���� ����
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 150)
		{
			if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ϳ� �÷��̾ ���� ���� �ִ�
				_isFindPlayer = true;
			}
			else if (static_cast<int>(_pointx / TILESIZE) < static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) < static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾��� ���� ���� �ִ�getMapInfo(int i, int j)
				int count = 0;
				for (int i = static_cast<int>(_pointx / TILESIZE); i < static_cast<int>(_player->getPoint().x); i++)
				{
					for (int j = static_cast<int>(_pointy / TILESIZE); j < static_cast<int>(_player->getPoint().y); j++)
					{
						if (_map->getMapInfo(j, i).type != 1) count++;
					}
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) < static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾� ���� �ִ�
				int count = 0;
				for (int j = static_cast<int>(_pointy / TILESIZE); j < static_cast<int>(_player->getPoint().y); j++)
				{
					if (_map->getMapInfo(j, static_cast<int>(_pointx / TILESIZE)).type != 1) count++;
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) > static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) < static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾� ������ ���� �ִ�
				int count = 0;
				for (int i = static_cast<int>(_player->getPoint().x); i < static_cast<int>(_pointx / TILESIZE); i++)
				{
					for (int j = static_cast<int>(_pointy / TILESIZE); j < static_cast<int>(_player->getPoint().y); j++)
					{
						if (_map->getMapInfo(j, i).type != 1) count++;
					}
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) < static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾��� ���ʿ� �ִ�
				int count = 0;
				for (int j = static_cast<int>(_pointx / TILESIZE); j < static_cast<int>(_player->getPoint().x); j++)
				{
					if (_map->getMapInfo(static_cast<int>(_pointy / TILESIZE), j).type != 1) count++;
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) > static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾� �����ʿ� �ִ�
				int count = 0;
				for (int j = static_cast<int>(_player->getPoint().x); j < static_cast<int>(_pointx / TILESIZE); j++)
				{
					if (_map->getMapInfo(static_cast<int>(_pointy / TILESIZE), j).type != 1) count++;
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) <static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE)> static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾��� ���� �Ʒ��� �ִ�
				int count = 0;
				for (int i = static_cast<int>(_pointx / TILESIZE); i < static_cast<int>(_player->getPoint().x); i++)
				{
					for (int j = static_cast<int>(_player->getPoint().y); j < static_cast<int>(_pointy / TILESIZE); j++)
					{
						if (_map->getMapInfo(j, i).type != 1) count++;
					}
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) > static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾� �Ʒ��� �ִ�
				int count = 0;
				for (int j = static_cast<int>(_player->getPoint().y); j < static_cast<int>(_pointy / TILESIZE); j++)
				{
					if (_map->getMapInfo(j, static_cast<int>(_pointx / TILESIZE)).type != 1) count++;
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
			}
			else if (static_cast<int>(_pointx / TILESIZE) > static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) > static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ͱ� �÷��̾� ������ �Ʒ��� �ִ�
				int count = 0;
				for (int i = static_cast<int>(_player->getPoint().x); i < static_cast<int>(_pointx / TILESIZE); i++)
				{
					
					for (int j = static_cast<int>(_player->getPoint().y); j < static_cast<int>(_pointy / TILESIZE); j++)
					{
						if (_map->getMapInfo(j, i).type != 1) count++;
					}
				}
				if (count > 0)
					_isFindPlayer = true;
				else _isFindPlayer = false;
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
	if ((_pointx > camera.x && _pointx < camera.x + WINSIZEX) &&
		_pointy > camera.y && _pointy < camera.y + WINSIZEY)
	{
		_image->frameRender(getMemDC(), _rc.left - camera.x, _rc.top - camera.y);
	}
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