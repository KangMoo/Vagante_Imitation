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

	//�ʿ���ȭ
	_image = IMAGEMANAGER->findImage("enemy_slime_idle");
	_state = ENEMYSTATE_IDLE;
	
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, 1, 1);

	//�߰����� �ʱ�ȭ
	jumpTime = 0.0f;
	distance = 0.5f;
	jumpPower = 15;
	saveY = 0;
	isJumped = false;
	jumped = false;

	_xspeed = 1;

	_actTimer = TIMEMANAGER->getWorldTime();
	_timerForFrameUpdate = TIMEMANAGER->getWorldTime();

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
	stateHandle(); // �߰�

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

		//if (!stand)
		//{
			_isFindPlayer = true;
			move();
			jump();
			attack();
		//}

		//if (stand)
		//{
		//	if (TIMEMANAGER->getWorldTime() - _timerForFrameUpdate > 0.5)
		//	{
		//		stand = false;
		//	}
		//}

		//���� �� ������ �Ÿ��� �Ѱ� �νĹ��� �̻����� �������� �Ѵ°� �����Ѵ�
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > _maxCog)
			_isFindPlayer = false;
	}

	//else
	if (!_isFindPlayer)
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
		}
	}

	if (_isFindPlayer)
	{
		int count = 0;
		float x = 0;
		float y = 0;
		float dist = getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
		float angle = getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
		//���� �� ���̿� ���� �ִ��� �Ǻ��Ѵ�
		for (int i = 0; i < dist; i += TILESIZE)
		{
			//���� ���� �˻��� Ÿ���� �̹� �˻��� Ÿ�ϰ� ���ٸ� �������� �Ѿ��
			float ox = (_pointx + i*cosf(angle)) / TILESIZE;
			float oy = (_pointy + i*-sinf(angle)) / TILESIZE;

			if (ox == x && oy == y) continue;

			//�ٸ��ٸ� x, y�� �ٲ��ְ� �� Ÿ���� �˻��Ѵ�
			x = ox;
			y = oy;

			int test = _map->getMapInfo(y, x).type;
			float p = 0;

			if (static_cast<int>(_map->getMapInfo(y, x).type == 1))
				count++;
		}
		//���� ���� �ϳ��� ����Ǿ��ٸ� �ν� �������� ó��
		if (count >= 1)
		{
			_isFindPlayer = false;
		}

		else
		{
			_isFindPlayer = true;
		}
	}

	//������ ������Ʈ�� rect�� �Ÿ��� ������� ó��
	if (TIMEMANAGER->getWorldTime() - _timerForFrameUpdate > 0.2)
	{
		frameUpdate();
		_timerForFrameUpdate = TIMEMANAGER->getWorldTime();
	}
	rectResize();
}
void Enemy::render()
{
	//_image->frameRender(getMemDC(), _rc.left, _rc.top);
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
	//if ((_pointx > camera.x && _pointx < camera.x + WINSIZEX) &&
	//	_pointy > camera.y && _pointy < camera.y + WINSIZEY)
	//{
	//	_image->frameRender(getMemDC(), _rc.left - camera.x, _rc.top - camera.y);
	//}

	//�߰�

	Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	_image->frameRender(getMemDC(), _pointx + camera.x, _pointy + camera.y, _currentFrameX, _currentFrameY);
}
void Enemy::move()
{
	if (_player->getPoint().x >= _pointx)
	{
		_pointx += _xspeed;
	}

	if (_player->getPoint().x <= _pointx)
	{
		_pointx -= _xspeed;
	}

	_pointy -= _yspeed;

	_rc = RectMakeCenter(_pointx, _pointy, 32, 32);
	//mapCollisionHandle();
	//_xspeed = 0;
	//_yspeed = 0;
}
void Enemy::jump()			
{
	if (isJumped == false)
		return;

	float height = jumpTime * jumpTime - jumpPower * jumpTime;

	jumpTime += distance;

	if (jumpTime > jumpPower)
	{
		isJumped = false;
	}

	(float)_pointy = saveY + height;

	if (jumped)
	{
		//if (_player->getState() != PLAYERSTATE_JUMPING && _player->getState() != PLAYERSTATE_FALLING)
		//{
		//	_player->getDamaged(5);
		//}

		jumped = false;
		_state = ENEMYSTATE_JUMPING;
		imageChange();
		_actTimer = TIMEMANAGER->getWorldTime();
	}
}
void Enemy::attack()		
{

}

void Enemy::frameUpdate()
{
	if (_player->getPoint().x > _pointx)
	{
		_currentFrameY = 0;
	}
	else
	{
		_currentFrameY = 1;
	}

	switch (_state)
	{
	case ENEMYSTATE_IDLE:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;

	case ENEMYSTATE_MOVING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;

	case ENEMYSTATE_JUMPING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	}
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
	_rc = RectMakeCenter(_pointx + (_image->getFrameWidth() / 2), _pointy + (_image->getFrameHeight() / 2), _image->getFrameWidth(), _image->getFrameHeight());
}

void Enemy::stateHandle()
{
	switch (_state)
	{
	case ENEMYSTATE_IDLE:
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			int random = rand() % 10;

			//if (random > 7)
			//{
			//	_state = ENEMYSTATE_IDLE;
			//	stand = true;
			//}

			if (random >= 3 && random < 7)
			{
				_state = ENEMYSTATE_MOVING;
			}

			if (random < 3)
			{
				_state = ENEMYSTATE_JUMPING;

				if (isJumped == false)
				{
					isJumped = true;
					jumpTime = 0.0f;

					saveY = _pointy;
				}
			}

			imageChange();
		}
		break;

	case ENEMYSTATE_MOVING:
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) >= _minCog)
		{
			_state = ENEMYSTATE_IDLE;
			imageChange();
		}
		break;

	case ENEMYSTATE_JUMPING:
		if (jumped == false)
		{
			jumped = true;
			_actTimer = TIMEMANAGER->getWorldTime();
		}

		else
		{
			if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) >= _minCog)
			{
				_state = ENEMYSTATE_IDLE;
			}

			else
			{
				_state = ENEMYSTATE_MOVING;
			}

			imageChange();
		}
		break;
	}
}

void Enemy::imageChange()
{
	//�߰�
	switch (_state)
	{
	case ENEMYSTATE_IDLE:
		_image = IMAGEMANAGER->findImage("enemy_slime_idle");
		break;
	case ENEMYSTATE_MOVING:
		_image = IMAGEMANAGER->findImage("enemy_slime_dash");
		break;
	case ENEMYSTATE_JUMPING:
		_image = IMAGEMANAGER->findImage("enemy_slime_jump");
		break;
	}
}
