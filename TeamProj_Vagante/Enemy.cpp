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
HRESULT Enemy::init(POINT point, float minCog, float maxCog)
{
	//�ڽ�Ŭ�������� ���� �ʱ�ȭ�ϱ�
	_minCog = minCog;
	_maxCog = maxCog;

	_pointx = point.x;
	_pointy = point.y;

	_frameFPS = 0;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;

	_xspeed = _yspeed = _angle = _gravity = 0;

	_money = 0;

	_isFindPlayer = false;
	//_statusEffect[5]
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}

	memset(&_statistics, 0, sizeof(tagStat));

	_state = ENEMYSTATE_IDLE;
	
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, 1, 1);

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
	//���ݿ� ��Ʈ �������ִ� �Լ�, ���� �������� �ֵ��� �׳� ���ݷ�Ʈ�� �Ȱ����� �״�� ó��
	//������Ͽ� �����ϱ� ���� ���� �ʿ��ϸ� ������Ұ�
	attRectClear();
	//�����̻�ȿ�� ó��
	statusEffect();
	//���� ó��
	falling();

	if (_isFindPlayer)
	{
		//���� �����̴� ��Ŀ������ �ٸ��Ƿ� �˾Ƽ� ó��
		_isFindPlayer = true;
		move();
		jump();
		attack();


		//���� �� ������ �Ÿ��� �Ѱ� �νĹ��� �̻����� �������� �Ѵ°� �����Ѵ�
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
				//���Ϳ� �÷��̾ ���� ���� �ִٸ� ���̵� ���� ó��
				_isFindPlayer = true;
			}
			else
			{
				int count = 0;
				float x = 0;
				float y = 0;
				float dist = getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
				float angle = getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
				//���� �� ���̿� ���� �ִ��� �Ǻ��Ѵ�
				for (int i = 0; i < dist; i+=TILESIZE)
				{
					//���� ���� �˻��� Ÿ���� �̹� �˻��� Ÿ�ϰ� ���ٸ� �������� �Ѿ��
					float ox = (_pointx + i*cosf(angle)) / TILESIZE;
					float oy = (_pointy + i*-sinf(angle)) / TILESIZE;
					
					if (ox == x && oy == y) continue;

					//�ٸ��ٸ� x, y�� �ٲ��ְ� �� Ÿ���� �˻��Ѵ�
					x = ox;
					y = oy;
						
					if (static_cast<int>(_map->getMapInfo(y, x).type == 1))
						count++;
				}
				//���� ���� �ϳ��� ����Ǿ��ٸ� �ν� �������� ó��
				if (count >= 1) _isFindPlayer = false;
				else _isFindPlayer = true;
			}
		}
	}
	//������ ������Ʈ�� rect�� �Ÿ��� ������� ó��
	frameUpdate();
	rectResize();
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
	//���� �þ߿� ���� ���� ������
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


void Enemy::statusEffect()
{
	//�������ͽ� �̻�� ȿ���� �ֱ�����
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

void Enemy::falling()
{

	/*
	�� ������ �ٸ� ó���� �ʿ��� ������ ���
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
	*/
}

void Enemy::rectResize()
{
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}