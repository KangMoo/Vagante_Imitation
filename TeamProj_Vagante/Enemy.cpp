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
	_lastPlayerPoint = _player->getPoint();
	_dead = false;
	_deadAlpha = 0;

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		getDamaged(10);
	}

	//���ݿ� ��Ʈ �������ִ� �Լ�, ���� �������� �ֵ��� �׳� ���ݷ�Ʈ�� �Ȱ����� �״�� ó��
	//������Ͽ� �����ϱ� ���� ���� �ʿ��ϸ� ������Ұ�
	attRectClear();
	//�����̻�ȿ�� ó��
	statusEffect();
	//���� ó��
	falling();
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}

	if (_isFindPlayer && _state != ENEMYSTATE_DEAD)
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
				playerCog();
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
	/*
	//Rectangle(getMemDC(), _pointx - _minCog / 2 + camera.x, _pointy - _minCog / 2 + camera.y, _pointx + _minCog / 2 + camera.x, _pointy + _minCog / 2 + camera.y);
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	if (_state != ENEMYSTATE_DEAD)
		_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y);
	else
		_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _deadAlpha);
	//EllipseMakeCenter(getMemDC(), _pointx + camera.x, _pointy + camera.y, 5, 5);

	
	char string[128];
	sprintf(string, "x : %d, y : %d", _rc.left / TILESIZE, _rc.top / TILESIZE);
	TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, string, strlen(string));
	if (_isFindPlayer)
	{
		sprintf(string, "����");
		TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	else
	{
		sprintf(string, "����");
		TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	*/
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
}

void Enemy::rectResize()
{
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void Enemy::playerCog()
{

	if (_lastPlayerPoint.x != _player->getPoint().x && _lastPlayerPoint.y != _player->getPoint().y)
	{
		_lastPlayerPoint = _player->getPoint();
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

			if (static_cast<int>(_map->getMapInfo(y, x).type == MAPTILE_WALL))
			{
				count++;
				break;
			}
		}
		//���� ���� �ϳ��� ����Ǿ��ٸ� �ν� �������� ó��
		if (count >= 1) _isFindPlayer = false;
		else _isFindPlayer = true;
	}
}