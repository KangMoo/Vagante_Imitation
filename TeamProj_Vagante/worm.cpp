#include "stdafx.h"
#include "worm.h"


worm::worm()
{
}


worm::~worm()
{
}
/*
struct tagStat {
	int hp;		//ü��
	int str;	//��
	int dex;	//��ø
	int vit;	//Ȱ��
	int inl;	//����
	int lck;	//��
	int def;	//����
	int fir;	//������
	int ice;	//��������
	int lgt;	//������
	int psn;	//������
	int mel;	//�������ݵ�����
	int rng;	//Ȱ���ݵ�����
	int crit;	//ũ��Ƽ��Ȯ��
	int aspd;	//����
	int spd;	//�̼�
};
*/

HRESULT worm::init(POINT point, float minCog, float maxCog)
{
	_image = IMAGEMANAGER->findImage("wormMoveUp");
	//�ڽ�Ŭ�������� ���� �ʱ�ȭ�ϱ�
	_minCog = minCog;
	_maxCog = maxCog;
	
	_pointx = point.x;
	_pointy = point.y;
	
	_frameFPS = 10.0f;
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
	
	int a = RND->getInt(2);

	if (a == 0) _isLeft = true;
	else _isLeft = false;
	
	_reverseFrame = false;
	
	_moveSpeed = 1;
	
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, 1, 1);

	_statistics.hp = 10;
	_statistics.str = 2;
	_statistics.dex = 2;
	_statistics.vit = 2;
	_statistics.inl = 2;
	_statistics.lck = 2;
	_statistics.def = 2;
	_statistics.fir = 0;
	_statistics.ice = 0;
	_statistics.lgt = 0;
	_statistics.psn = 0;
	_statistics.mel = 0;
	_statistics.rng = 0;
	_statistics.crit = 0;
	_statistics.aspd = 0;
	_statistics.spd = 1;

	//0 = �ٴ�, 1 = ���ʺ�, 2 = ���ʺ�, 3 = �����ʺ�
	_whereIsWorm = 0;
	
	return S_OK;
}

void worm::move()
{
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
	{
		_state = ENEMYSTATE_MOVING;
		//0->�ٴ�, 1->�޺�, 2->����, 3->������
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != 1)
				{
					//���� �ٷ� �հ� �տ� �Ʒ����� ������
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == 1)
				{
					//�ٴڿ��� �������� �� ��, ���� �ٷ� �տ� �ִ� ���
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.bottom;
				}
				else
				{
					_pointx -= _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx + _moveSpeed) / TILESIZE).type != 1)
				{
					//���� �ٷ� �հ� ���� �Ʒ����� ������
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == 1)
				{
					//���� �ٷ� �տ� ���� ���
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.bottom;
				}
				else
				{
					_pointy += _moveSpeed;
				}
			}
			break;
		case 1:
			if (_isLeft)
			{
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != 1)
				{
					//�ٷ� ���� ���� ���� ���� �� �Ʒ��� ���� ���
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - _image->getFrameHeight() / 2;
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == 1)
				{
					//�ٷ� �տ� ���� ������
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _image->getFrameHeight() / 2;
					_pointx = _map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.left;
				}
				else
				{
					_pointy -= _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != 1)
				{
					//�տ� �� ���� �Ʒ��� �� ����
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.bottom + _image->getFrameHeight() / 2;
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.left;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == 1)
				{
					//�տ� �� ����
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.top - _image->getFrameHeight() / 2;
					_pointx = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.right;
				}
				else
				{
					_pointy += _moveSpeed;
				}
			}
			break;
		case 2:
			if (_isLeft)
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != 1)
				{
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == 1)
				{
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.top;
				}
				else
				{
					_pointx += _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx - _moveSpeed) / TILESIZE).type != 1)
				{
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE+1, (_pointx) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == 1)
				{
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + _image->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.bottom;
				}
				else
				{
					_pointx -= _moveSpeed;
				}
			}
			break;
		case 3:
			if (_isLeft)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != 1)
				{
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx ) / TILESIZE + 1).rc.bottom + _image->getFrameHeight() / 2;
					_pointx= _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == 1)
				{
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.top - _image->getFrameHeight() / 2;
					_pointx = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.right;
				}
				else
				{
					_pointy += _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != 1 &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != 1)
				{
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.top - _image->getFrameHeight() / 2;
					_pointx= _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.right;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == 1)
				{
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _image->getFrameHeight() / 2;
					_pointx = _map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.left;
				}
				else
				{
					_pointy -= _moveSpeed;
				}
			}
			break;
		}
	}
}
void worm::attack()
{
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
		_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	else
		_attackRect = RectMakeCenter(_pointx, _pointy, 0, 0);
}
void worm::frameUpdate()
{
	_frameFPS = 10;
	/*
	switch (_state)
	{
	case ENEMYSTATE_IDLE: case ENEMYSTATE_MOVING:
		switch (_whereIsWorm)
		{
		case 0:
			_image = IMAGEMANAGER->findImage("wormMoveUp");
			break;
		case 1:
			_image = IMAGEMANAGER->findImage("wormMoveRight");
			break;
		case 2:
			_image = IMAGEMANAGER->findImage("wormMoveDown");
			break;
		case 3:
			_image = IMAGEMANAGER->findImage("wormMoveLeft");
			break;
		default:
			_whereIsWorm = 0;
			break;
		}
		/*
		if (_botM.type == 1)
			_image = IMAGEMANAGER->findImage("wormMoveUp");
		else if (_upM.type == 1)
			_image = IMAGEMANAGER->findImage("wormMoveDown");
		else if (_midL.type == 1)
			_image = IMAGEMANAGER->findImage("wormMoveRight");
		else if (_midR.type == 1)
			_image = IMAGEMANAGER->findImage("wormMoveLeft");
		//_image = IMAGEMANAGER->findImage("wormMove");
		
		break;
	case ENEMYSTATE_HIT: case ENEMYSTATE_FALLING: case ENEMYSTATE_DEAD:
		_image = IMAGEMANAGER->findImage("wormHit");
		break;
	}
	*/
	if (_state == ENEMYSTATE_MOVING)
	{
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 1:
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 2:
			if (_isLeft)
				_image->setFrameY(1);
			else _image->setFrameY(0);
			break;
		case 3:
			if (_isLeft)
				_image->setFrameY(1);
			else _image->setFrameY(0);
			break;
		}
		if (_isLeft)
			_image->setFrameY(0);
		else _image->setFrameY(1);
		_frameTime ++;
		if (_frameTime >= _frameFPS)
		{
			_frameTime = 0;
			if (_reverseFrame)
				_currentFrameX--;
			else
				_currentFrameX++;
			if (_currentFrameX > _image->getMaxFrameX())
			{
				_reverseFrame = true;
				_currentFrameX = _image->getMaxFrameX();
			}
			else if (_currentFrameX < 0)
			{
				_reverseFrame = false;
				_currentFrameX = 0;
			}
			_image->setFrameX(_currentFrameX);
		}
	}
	else
	{
		_currentFrameX = 0;
		_frameFPS = 0;
		_image->setFrameX(0);
		if (_isLeft)
			_image->setFrameY(0);
		else
			_image->setFrameY(1);
	}
	/*
	ENEMYSTATE_IDLE,
		ENEMYSTATE_MOVING,
		ENEMYSTATE_ATTACKING,
		ENEMYSTATE_HIT,
		ENEMYSTATE_JUMPING,
		ENEMYSTATE_FALLING,
		ENEMYSTATE_DEAD
		*/
}

void worm::falling()	
{
	if (_state == ENEMYSTATE_HIT)
	{

	}
}

void worm::rectResize()	
{
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}