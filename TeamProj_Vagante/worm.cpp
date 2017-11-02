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
	int hp;		//체력
	int str;	//힘
	int dex;	//민첩
	int vit;	//활력
	int inl;	//지능
	int lck;	//운
	int def;	//방어력
	int fir;	//불저항
	int ice;	//얼음저항
	int lgt;	//빛저항
	int psn;	//독저항
	int mel;	//근접공격데미지
	int rng;	//활공격데미지
	int crit;	//크리티컬확률
	int aspd;	//공속
	int spd;	//이속
};
*/

HRESULT worm::init(POINT point, float minCog, float maxCog)
{
	_image = IMAGEMANAGER->findImage("wormMoveUp");
	//자식클래스에서 각자 초기화하기
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

	//0 = 바닥, 1 = 왼쪽벽, 2 = 위쪽벽, 3 = 오른쪽벽
	_whereIsWorm = 0;
	
	return S_OK;
}

void worm::move()
{
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
	{
		_state = ENEMYSTATE_MOVING;
		//0->바닥, 1->왼벽, 2->위벽, 3->오른벽
		/*
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
			{
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//벽이 바로 앞과 앞에 아래에도 없을때
					_whereIsWorm = 3;
					_image = IMAGEMANAGER->findImage("wormMoveLeftt");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - _image->getFrameWidth() / 2;
					_pointx -= 1;
					
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//바닥에서 왼쪽으로 갈 때, 벽이 바로 앞에 있는 경우
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
					//벽이 바로 앞과 앞의 아래에도 없을때
					_whereIsWorm = 1;
					_image = IMAGEMANAGER->findImage("wormMoveRight");
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _image->getFrameWidth() / 2;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//벽이 바로 앞에 없는 경우
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
					//바로 앞쪽 위에 벽이 없고 그 아래도 없는 경우
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveUp");
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - _image->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//바로 앞에 벽이 있을때
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
					//앞에 벽 없고 아래도 벽 없음
					_whereIsWorm = 2;
					_image = IMAGEMANAGER->findImage("wormMoveDown");
					_pointy = _map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).rc.top + _image->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞에 벽 있음
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
		//_whereIsWorm은 벽이 어디 붙어있는지 판단하는 변수입니다
		//0은 바닥(이미지 wormMoveUp), 1은 왼쪽벽(이미지 wormMoveRight), 2는 윗쪽벽(이미지 wormMoveDown), 3은 오른쪽벽(이미지 wormMoveLeft)
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
	//_whereIsWorm은 벽이 어디 붙어있는지 판단하는 변수입니다
	//0은 바닥(이미지 wormMoveUp), 1은 왼쪽벽(이미지 wormMoveRight), 2는 윗쪽벽(이미지 wormMoveDown), 3은 오른쪽벽(이미지 wormMoveLeft)
	//현재 진행방향 앞에 벽이 있는지, 앞에 벽이 없는데 그 아래도 벽이 없는지 판별합니다
	//현재 있는 타일 rect의 끝에 도달했는지 먼저 검사합시다
	switch (_whereIsWorm)
	{
	case 0:
		if (_isLeft)
		{
			//이번에 움직일 때 중심이 타일을 넘기는지 체크
			if ((_pointx - _moveSpeed)/TILESIZE < (_pointx)/TILESIZE)
			{
				//넘겼으면 변환을 시작
				//먼저 앞쪽에 벽이 있는지 계산
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//벽이 있다면 그 벽을 타고 올라가게 바꿔준다
					//모션은 1번(wormMoveRight)
					_whereIsWorm = 1;

					//x, y값을 보정한다
					//x는 앞쪽 벽(x-1) rect의 오른쪽에서 이미지 절반값만큼 더하고, y는 rect의 아래쪽에서 이속만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//만약 앞쪽에 벽이 없는데 그 아래에도 벽이 없을 경우 지금 자기 아래 타일을 타고 내려가야한다
					//x는 그대로, y는 + 1 해준 자기 앞 아래의 타일 검사, 또 자기 바로 앞 타일 검사
					//없으면 타고 내려간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 자기가 밟고있는 바닥(y+1)rect 왼쪽에서 이미지 절반값만큼 빼주고, y는 자기가 밟고있는 바닥rect 위에서 이속만큼 더해준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		else
		{
			//움직일 때 중심이 타일을 넘기는지 체크, _isLeft가 true일 때와는 다르다
			if ((_pointx + _moveSpeed) > (_pointx)/TILESIZE)
			{
				//역시 넘겼으면 변환을 시작
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으면 타고 올라간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 앞쪽 벽(x+1) rect의 왼쪽에서 이미지 절반값만큼 빼주고, y는 앞쪽 벽 rect의 아래쪽에서 이속만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy)/TILESIZE + 1, (_pointx + _moveSpeed)/TILESIZE).type != MAPTILE_WALL)
				{
					//역시 앞쪽에 벽 없는지 체크하고 아래도 체크해준다
					//둘다 없으면 1번(wormMoveRight)
					_whereIsWorm = 1;
					//값보정
					//x는 자기가 밟고있는 바닥(y+1)rect의 오른쪽에서 이미지 절반값만큼 더해주고, y는 자기가 밟고있는 바닥rect의 윗쪽에서 이속만큼 더한다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		break;
	case 1:
		
		if (_isLeft)
		{
			//중심을 넘기는지 체크, 이번엔 y값을 체크한다
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//넘겼으면 변환 시작
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으니 올라탄다
					//천장에 붙으므로 2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 위쪽 벽(y-1)rect의 왼쪽에서 이속만큼 더해주고, y는 위쪽벽 rect의 아래쪽에서 이미지 절반값만큼 더해준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight()/2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//위쪽에도 벽이 없고 그 아래에도 벽이 없으니 꺾인다
					//바닥에 붙으니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 밟고있는 바닥(x-1)rect의 오른쪽에서 이속만큼 빼주고, y는 밟고있는 바닥rect의 위쪽에서 이미지 절반값만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//지금 있는 타일보다 내려가는지 체크
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있다면 안착하니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 벽(y+1) rect의 왼쪽값에서 이속만큼 더해주고, y는 벽rect의 위쪽에서 이미지 절반값만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//아래쪽에도 벽이 없고 그 왼쪽에도 없으니 천장
					//2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 자기가 밟고있는 벽(x-1) rect의 오른쪽에서 이속만큼 빼주고, y는 밟고있는 벽 rect의 아래에서 이미지 절반값만큼 더해준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		
		break;
	case 2:
		//0번과 기본은 같다
		if (_isLeft)
		{
			//움직일 때 중심이 타일을 넘기는지 체크, _isLeft가 true일 때와는 다르다
			if ((_pointx + _moveSpeed) / TILESIZE > (_pointx) / TILESIZE)
			{
				//역시 넘겼으면 변환을 시작
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으면 타고 올라간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 앞쪽벽(x+1)의 왼쪽에서 이미지 절반값만큼 -, y는 위에서 이속만큼 +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth()/2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//역시 앞쪽에 벽 없는지 체크하고 아래도 체크해준다
					//둘다 없으면 1번(wormMoveRight)
					_whereIsWorm = 1;
					//값보정
					//x는 자기가 밟은벽(y - 1)의 오른쪽에서 이미지절반+, y는 자기가 밟은벽의 아래에서 이속만큼 빼줌
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom - _moveSpeed;
				}
			}
		}
		else
		{
			//이번에 움직일 때 중심이 타일을 넘기는지 체크
			if ((_pointx - _moveSpeed) / TILESIZE < (_pointx) / TILESIZE)
			{
				//넘겼으면 변환을 시작
				//먼저 앞쪽에 벽이 있는지 계산
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//벽이 있다면 그 벽을 타고 올라가게 바꿔준다
					//모션은 1번(wormMoveRight)
					_whereIsWorm = 1;

					//x, y값을 보정한다
					//x는 앞쪽벽(x-1)의 오른쪽에서 이미지 절반+, y는 앞쪽벽 위에서 이속만큼 +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).type != MAPTILE_WALL)
				{
					//만약 앞쪽에 벽이 없는데 그 아래에도 벽이 없을 경우 지금 자기 아래 타일을 타고 내려가야한다
					//x는 그대로, y는 + 1 해준 자기 앞 아래의 타일 검사, 또 자기 바로 앞 타일 검사
					//없으면 타고 내려간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 밟은벽(y-1)왼쪽에서 이미지절반만큼-, y는 밟은벽 아래서 이속만큼-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + _moveSpeed;
				}
			}
		}
		break;
	case 3:
		if (_isLeft)
		{
			//지금 있는 타일보다 내려가는지 체크
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있다면 안착하니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 앞쪽벽(y+1)의 오른쪽에서 이속만큼-, y는 위에서 이미지절반만큼-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//아래쪽에도 벽이 없고 그 왼쪽에도 없으니 천장
					//2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 밟은벽(x + 1)왼쪽에서 이속만큼+, y는 아래에서 이미지 절반만큼+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//중심을 넘기는지 체크, 이번엔 y값을 체크한다
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//넘겼으면 변환 시작
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으니 올라탄다
					//천장에 붙으므로 2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 앞쪽벽(y-1)의 오른쪽에서 이속만큼-, y는 아래에서 이미지 절반만큼+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//위쪽에도 벽이 없고 그 아래에도 벽이 없으니 꺾인다
					//바닥에 붙으니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 밟은벽(x+1)의 왼쪽에서 이속만큼+, y는 위에서 이미지 절반만큼 -
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		break;
	}
}