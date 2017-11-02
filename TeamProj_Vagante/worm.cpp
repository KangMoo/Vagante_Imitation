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

	_moveLeft = new image;
	_moveLeft->init("Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveRight = new image;
	_moveRight->init("Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveUp = new image;
	_moveUp->init("Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveDown = new image;
	_moveDown->init("Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));

	/*
	IMAGEMANAGER->addFrameImage("wormMoveUp", "Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveDown", "Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveLeft", "Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveRight", "Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	*/

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
	//_isLeft = true;
	
	_reverseFrame = false;
	
	_moveSpeed = 0.5;
	
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
		/*
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� �ٷ� �հ� �տ� �Ʒ����� ������
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveLeftt");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
					_pointx -= 1;
					
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//�ٴڿ��� �������� �� ��, ���� �ٷ� �տ� �ִ� ���
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + _image->getFrameWidth() / 2;
				}
				else
				{
					_pointx -= _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� �ٷ� �հ� ���� �Ʒ����� ������
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _image->getFrameWidth() / 2;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���� �ٷ� �տ� ���� ���
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
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
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//�ٷ� ���� ���� ���� ���� �� �Ʒ��� ���� ���
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - _image->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//�ٷ� �տ� ���� ������
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _image->getFrameHeight() / 2;
				}
				else
				{
					_pointy -= _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//�տ� �� ���� �Ʒ��� �� ����
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).rc.top + _image->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//�տ� �� ����
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.top - _image->getFrameHeight() / 2;
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
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + _image->getFrameWidth() / 2;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
				}
				else
				{
					_pointx += _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveLeft");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE+1, (_pointx) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + _image->getFrameWidth() / 2;
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
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx ) / TILESIZE + 1).rc.bottom + _image->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.top - _image->getFrameHeight() / 2;
				}
				else
				{
					_pointy += _moveSpeed;
				}
			}
			else
			{
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.top - _image->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					_whereIsWorm = 0;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _image->getFrameHeight() / 2;
				}
				else
				{
					_pointy -= _moveSpeed;
				}
			}
			break;
		}
		*/
		//_whereIsWorm�� ���� ��� �پ��ִ��� �Ǵ��ϴ� �����Դϴ�
		//0�� �ٴ�(�̹��� wormMoveUp), 1�� ���ʺ�(�̹��� wormMoveRight), 2�� ���ʺ�(�̹��� wormMoveDown), 3�� �����ʺ�(�̹��� wormMoveLeft)
		isThereWall();
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
				_pointx -= _moveSpeed;
			else
				_pointx += _moveSpeed;
			break;
		case 1:
			if (_isLeft)
				_pointy -= _moveSpeed;
			else
				_pointy += _moveSpeed;
			break;
		case 2:
			if (_isLeft)
				_pointx += _moveSpeed;
			else
				_pointx -= _moveSpeed;
			break;
		case 3:
			if (_isLeft)
				_pointy += _moveSpeed;
			else
				_pointy -= _moveSpeed;
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
			//_image = IMAGEMANAGER->findImage("wormMoveUp");
			_image = _moveUp;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 1:
			//_image = IMAGEMANAGER->findImage("wormMoveRight");
			_image = _moveRight;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 2:
			//_image = IMAGEMANAGER->findImage("wormMoveDown");
			_image = _moveDown;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 3:
			//_image = IMAGEMANAGER->findImage("wormMoveLeft");
			_image = _moveLeft;
			if (_isLeft)
				_image->setFrameY(1);
			else _image->setFrameY(0);
			break;
		}
		//if (_isLeft)
		//	_image->setFrameY(0);
		//else _image->setFrameY(1);
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
				_currentFrameX = 1;
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

void worm::isThereWall()
{
	//_whereIsWorm�� ���� ��� �پ��ִ��� �Ǵ��ϴ� �����Դϴ�
	//0�� �ٴ�(�̹��� wormMoveUp), 1�� ���ʺ�(�̹��� wormMoveRight), 2�� ���ʺ�(�̹��� wormMoveDown), 3�� �����ʺ�(�̹��� wormMoveLeft)
	//���� ������� �տ� ���� �ִ���, �տ� ���� ���µ� �� �Ʒ��� ���� ������ �Ǻ��մϴ�
	//���� �ִ� Ÿ�� rect�� ���� �����ߴ��� ���� �˻��սô�
	switch (_whereIsWorm)
	{
	case 0:
		if (_isLeft)
		{
			//�̹��� ������ �� �߽��� Ÿ���� �ѱ���� üũ
			if ((_pointx - _moveSpeed)/TILESIZE < (_pointx)/TILESIZE)
			{
				//�Ѱ����� ��ȯ�� ����
				//���� ���ʿ� ���� �ִ��� ���
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���� �ִٸ� �� ���� Ÿ�� �ö󰡰� �ٲ��ش�
					//����� 1��(wormMoveRight)
					_whereIsWorm = 1;

					//x, y���� �����Ѵ�
					//x�� ���� ��(x-1) rect�� �����ʿ��� �̹��� ���ݰ���ŭ ���ϰ�, y�� rect�� �Ʒ��ʿ��� �̼Ӹ�ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� ���� ���µ� �� �Ʒ����� ���� ���� ��� ���� �ڱ� �Ʒ� Ÿ���� Ÿ�� ���������Ѵ�
					//x�� �״��, y�� + 1 ���� �ڱ� �� �Ʒ��� Ÿ�� �˻�, �� �ڱ� �ٷ� �� Ÿ�� �˻�
					//������ Ÿ�� ��������
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� �ڱⰡ ����ִ� �ٴ�(y+1)rect ���ʿ��� �̹��� ���ݰ���ŭ ���ְ�, y�� �ڱⰡ ����ִ� �ٴ�rect ������ �̼Ӹ�ŭ �����ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		else
		{
			//������ �� �߽��� Ÿ���� �ѱ���� üũ, _isLeft�� true�� ���ʹ� �ٸ���
			if ((_pointx + _moveSpeed) > (_pointx)/TILESIZE)
			{
				//���� �Ѱ����� ��ȯ�� ����
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ Ÿ�� �ö󰣴�
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� ���� ��(x+1) rect�� ���ʿ��� �̹��� ���ݰ���ŭ ���ְ�, y�� ���� �� rect�� �Ʒ��ʿ��� �̼Ӹ�ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy)/TILESIZE + 1, (_pointx + _moveSpeed)/TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� �� ������ üũ�ϰ� �Ʒ��� üũ���ش�
					//�Ѵ� ������ 1��(wormMoveRight)
					_whereIsWorm = 1;
					//������
					//x�� �ڱⰡ ����ִ� �ٴ�(y+1)rect�� �����ʿ��� �̹��� ���ݰ���ŭ �����ְ�, y�� �ڱⰡ ����ִ� �ٴ�rect�� ���ʿ��� �̼Ӹ�ŭ ���Ѵ�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		break;
	case 1:
		
		if (_isLeft)
		{
			//�߽��� �ѱ���� üũ, �̹��� y���� üũ�Ѵ�
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//�Ѱ����� ��ȯ ����
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ �ö�ź��
					//õ�忡 �����Ƿ� 2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� ���� ��(y-1)rect�� ���ʿ��� �̼Ӹ�ŭ �����ְ�, y�� ���ʺ� rect�� �Ʒ��ʿ��� �̹��� ���ݰ���ŭ �����ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight()/2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//���ʿ��� ���� ���� �� �Ʒ����� ���� ������ ���δ�
					//�ٴڿ� ������ 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ����ִ� �ٴ�(x-1)rect�� �����ʿ��� �̼Ӹ�ŭ ���ְ�, y�� ����ִ� �ٴ�rect�� ���ʿ��� �̹��� ���ݰ���ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//���� �ִ� Ÿ�Ϻ��� ���������� üũ
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� �ִٸ� �����ϴ� 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ��(y+1) rect�� ���ʰ����� �̼Ӹ�ŭ �����ְ�, y�� ��rect�� ���ʿ��� �̹��� ���ݰ���ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//�Ʒ��ʿ��� ���� ���� �� ���ʿ��� ������ õ��
					//2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� �ڱⰡ ����ִ� ��(x-1) rect�� �����ʿ��� �̼Ӹ�ŭ ���ְ�, y�� ����ִ� �� rect�� �Ʒ����� �̹��� ���ݰ���ŭ �����ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		
		break;
	case 2:
		//0���� �⺻�� ����
		if (_isLeft)
		{
			//������ �� �߽��� Ÿ���� �ѱ���� üũ, _isLeft�� true�� ���ʹ� �ٸ���
			if ((_pointx + _moveSpeed) / TILESIZE > (_pointx) / TILESIZE)
			{
				//���� �Ѱ����� ��ȯ�� ����
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ Ÿ�� �ö󰣴�
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� ���ʺ�(x+1)�� ���ʿ��� �̹��� ���ݰ���ŭ -, y�� ������ �̼Ӹ�ŭ +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth()/2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� �� ������ üũ�ϰ� �Ʒ��� üũ���ش�
					//�Ѵ� ������ 1��(wormMoveRight)
					_whereIsWorm = 1;
					//������
					//x�� �ڱⰡ ������(y - 1)�� �����ʿ��� �̹�������+, y�� �ڱⰡ �������� �Ʒ����� �̼Ӹ�ŭ ����
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom - _moveSpeed;
				}
			}
		}
		else
		{
			//�̹��� ������ �� �߽��� Ÿ���� �ѱ���� üũ
			if ((_pointx - _moveSpeed) / TILESIZE < (_pointx) / TILESIZE)
			{
				//�Ѱ����� ��ȯ�� ����
				//���� ���ʿ� ���� �ִ��� ���
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���� �ִٸ� �� ���� Ÿ�� �ö󰡰� �ٲ��ش�
					//����� 1��(wormMoveRight)
					_whereIsWorm = 1;

					//x, y���� �����Ѵ�
					//x�� ���ʺ�(x-1)�� �����ʿ��� �̹��� ����+, y�� ���ʺ� ������ �̼Ӹ�ŭ +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� ���� ���µ� �� �Ʒ����� ���� ���� ��� ���� �ڱ� �Ʒ� Ÿ���� Ÿ�� ���������Ѵ�
					//x�� �״��, y�� + 1 ���� �ڱ� �� �Ʒ��� Ÿ�� �˻�, �� �ڱ� �ٷ� �� Ÿ�� �˻�
					//������ Ÿ�� ��������
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� ������(y-1)���ʿ��� �̹������ݸ�ŭ-, y�� ������ �Ʒ��� �̼Ӹ�ŭ-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + _moveSpeed;
				}
			}
		}
		break;
	case 3:
		if (_isLeft)
		{
			//���� �ִ� Ÿ�Ϻ��� ���������� üũ
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� �ִٸ� �����ϴ� 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ���ʺ�(y+1)�� �����ʿ��� �̼Ӹ�ŭ-, y�� ������ �̹������ݸ�ŭ-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//�Ʒ��ʿ��� ���� ���� �� ���ʿ��� ������ õ��
					//2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� ������(x + 1)���ʿ��� �̼Ӹ�ŭ+, y�� �Ʒ����� �̹��� ���ݸ�ŭ+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//�߽��� �ѱ���� üũ, �̹��� y���� üũ�Ѵ�
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//�Ѱ����� ��ȯ ����
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ �ö�ź��
					//õ�忡 �����Ƿ� 2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� ���ʺ�(y-1)�� �����ʿ��� �̼Ӹ�ŭ-, y�� �Ʒ����� �̹��� ���ݸ�ŭ+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//���ʿ��� ���� ���� �� �Ʒ����� ���� ������ ���δ�
					//�ٴڿ� ������ 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ������(x+1)�� ���ʿ��� �̼Ӹ�ŭ+, y�� ������ �̹��� ���ݸ�ŭ -
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		break;
	}
}