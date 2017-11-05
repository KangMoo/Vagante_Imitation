#include "stdafx.h"
#include "slime.h"
#include "Player.h"
slime::slime()
{
}


slime::~slime()
{
}

HRESULT slime::init(POINT point, float minCog, float maxCog)
{
	//_image = IMAGEMANAGER->findImage("wormMoveUp");
	//자식클래스에서 각자 초기화하기
	//인식,최대
	_minCog = minCog;
	_maxCog = maxCog;

	//최초 생성위치
	_pointx = point.x;
	_pointy = point.y;


	_xspeed = _yspeed = _angle = _gravity = 0;
	//뿌릴 돈
	_money = RND->getFromIntTo(5, 1);

	//박쥐 스탯 임의로 때려박기
	_statistics.hp = 20;
	_statistics.str = 7;
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

	//죽었는지 확인용
	_dead = false;
	_deadAlpha = 0;
	_rc = RectMakeCenter(_pointx, _pointy, 25, 25);
	_isFindPlayer = false;	//플레이어 탐지여부
	_isOnTop = false;			//천장에 닿았는지 여부
	_image = IMAGEMANAGER->findImage("slime_Idle");
	_currentFrameX = 0;
	_currentFrameY = 0;
	_jumptimer = TIMEMANAGER->getWorldTime();
	_timerForFrame = TIMEMANAGER->getWorldTime();
	_slimeState = SLIMESTATE_IDLE;
	_isOnLand = false;
	return S_OK;
}

void slime::update()
{

	if (_slimeState != SLIMESTATE_DEAD)
	{
		//인식범위 내에 플레이어가 있으면 활성화, 아닐 시 비활성화
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 400) _isFindPlayer = true;
		else _isFindPlayer = false;
		actByState();


		mapCollisionCheck();
		move();
		mapCollisionCheck();
		hitPlayer();
		deadcheck();
		imgHandleByState();
		if (TIMEMANAGER->getWorldTime() - _timerForFrame > 0.2)
		{
			_timerForFrame = TIMEMANAGER->getWorldTime();
			frameUpdate();
		}
		_rc = RectMakeCenter(_pointx, _pointy, 25, 25);
	}
	else
	{
		mapCollisionCheck();
		move();
		mapCollisionCheck();
		deadcheck();
		imgHandleByState();
		if (TIMEMANAGER->getWorldTime() - _timerForFrame > 0.2)
		{
			_timerForFrame = TIMEMANAGER->getWorldTime();
			frameUpdate();
		}
		_rc = RectMakeCenter(_pointx, _pointy, 25, 25);
	}
	
}

void slime::actByState()
{
	switch (_slimeState)
	{
	case SLIMESTATE_IDLE:
		//플레이어 포착시
		if (_isFindPlayer)
		{
			//움직임
			_slimeState = SLIMESTATE_MOVING;
		}
		break;
	case SLIMESTATE_MOVING:
		//플레이어 포착 시
		if (_isFindPlayer)
		{
			//플레이어 방향으로 움직임(x축 무빙)
			if (_player->getPoint().x > _pointx)
			{
				//속도제한
				if (abs(_xspeed) < 1)
					_xspeed += 1;
			}
			else
			{
				if (abs(_xspeed) < 1)
					_xspeed -= 1;
			}
			//플레이어가 슬라임보다 위에 있을 시
			if (_pointy > _player->getPoint().y)
			{
				//점프 쿨타임 돌면~
				if (TIMEMANAGER->getWorldTime() - _jumptimer > 4)
				{
					_yspeed -= 10;
					_slimeState = SLIMESTATE_JUMPING;
					_jumptimer = TIMEMANAGER->getWorldTime();
				}
			}
		}
		break;
	case SLIMESTATE_JUMPING:
		if (_yspeed > 0) _slimeState = SLIMESTATE_FALLING;
		//플레이어 포착 시
		if (_isFindPlayer)
		{
			//플레이어 방향으로 움직임(x축 무빙)
			if (_player->getPoint().x > _pointx)
			{
				//속도제한
				if (abs(_xspeed) < 1)
					_xspeed += 1;
			}
			else
			{
				if (abs(_xspeed) < 1)
					_xspeed -= 1;
			}
		}
		break;
	case SLIMESTATE_FALLING:
		//플레이어 포착 시
		if (_isFindPlayer)
		{
			//플레이어 방향으로 움직임(x축 무빙)
			if (_player->getPoint().x > _pointx)
			{
				//속도제한
				if (abs(_xspeed) < 1)
					_xspeed += 1;
			}
			else
			{
				if (abs(_xspeed) < 1)
					_xspeed -= 1;
			}
		}
		if (_isOnLand)  _slimeState = SLIMESTATE_IDLE;
		break;
	case SLIMESTATE_DEAD:
		break;
	}
}

void slime::render(POINT camera)
{
	draw(camera);
}
void slime::draw(POINT camera)
{
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	_image->alphaFrameRender(getMemDC(),
		_pointx - _image->getFrameWidth() / 2 + camera.x,
		_pointy - _image->getFrameHeight() / 2 + camera.y,
		_currentFrameX, _currentFrameY, _deadAlpha);
}
void slime::move()
{
	_pointx += _xspeed;
	_pointy += _yspeed;

	//속도 감속
	if (_xspeed > 0) _xspeed -= 0.1;
	else if (_xspeed < 0) _xspeed += 0.1;
	if (_yspeed > 0) _yspeed -= 0.1;
	else if (_yspeed < 0) _yspeed += 0.1;

	//속도 한계치
	if (_xspeed > 2) _xspeed = 2;
	else if (_xspeed < -2) _xspeed = -2;

	//중력처리
	_yspeed += 0.3;
}

void slime::hitPlayer()
{
	RECT temp;
	//충돌시
	if (IntersectRect(&temp, &_player->getRect(), &_rc))
	{
		//플레이어 데미지 주기
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 3);
		//플레이어 반대방향으로 튕겨나기
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
	}
}
void slime::mapCollisionCheck()
{
	upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);


	_rc = RectMakeCenter(_pointx, _pointy, 25, 25);
	RECT temp;
	if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &midL.rc, &_rc))
	{
		_pointx += temp.right - temp.left;
	}
	if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &midR.rc, &_rc))
	{
		_pointx -= temp.right - temp.left;
	}
	if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &upM.rc, &_rc))
	{
		_pointy += temp.bottom - temp.top;
	}
	if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &botM.rc, &_rc))
	{
		_pointy -= temp.bottom - temp.top;
		_yspeed = 0;
		_isOnLand = true;
	}
	_rc = RectMakeCenter(_pointx, _pointy, 25, 25);
	
}
void slime::deadcheck()
{
	if (_statistics.hp <= 0)
	{
		_slimeState = SLIMESTATE_DEAD;
	}
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}
}
void slime::imgHandleByState()
{
	switch (_slimeState)
	{
	case SLIMESTATE_IDLE:
		_image = IMAGEMANAGER->findImage("slime_Idle");
		break;
	case SLIMESTATE_MOVING:
		_image = IMAGEMANAGER->findImage("slime_Move");
		break;
	case SLIMESTATE_JUMPING:
		_image = IMAGEMANAGER->findImage("slime_Jump");
		break;
	case SLIMESTATE_FALLING:
		_image = IMAGEMANAGER->findImage("slime_Falling");
		break;
	case SLIMESTATE_DEAD:
		_image = IMAGEMANAGER->findImage("slime_Idle");
		break;
	}
	if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
}
void slime::frameUpdate()
{
	switch (_slimeState)
	{
	case SLIMESTATE_IDLE:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case SLIMESTATE_MOVING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case SLIMESTATE_JUMPING:
		if (_currentFrameX >= _image->getMaxFrameX()) {}
		else _currentFrameX++;
		break;
	case SLIMESTATE_FALLING:

		break;
	case SLIMESTATE_DEAD:
		break;
	}
}