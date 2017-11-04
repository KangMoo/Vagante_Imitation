#include "stdafx.h"
#include "bat.h"
#include "Player.h"

bat::bat()
{
}


bat::~bat()
{
}


HRESULT bat::init(POINT point, float minCog, float maxCog)
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
	_statistics.str = 5;
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
	_deadAlpha = 255;
	_rc = RectMakeCenter(_pointx, _pointy, 10, 10);
	_isPlayerOnTarget = true;	//플레이어 탐지여부
	_isOnTop = false;			//천장에 닿았는지 여부
	_alpha = 0;
	_image = IMAGEMANAGER->findImage("batflying");
	_currentFrameX = 0;
	_currentFrameY = 0;
	_timerForFrame = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void bat::update() {

	//인식범위 내에 플레이어가 있으면 활성화, 아닐 시 비활성화
	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 200) _isPlayerOnTarget = true;
	else _isPlayerOnTarget = false;
	actByState();
	move();
	mapCollisionCheck();

	hitPlayer();
	deadcheck();
	imgHandleByState();
	if (TIMEMANAGER->getWorldTime() - _timerForFrame > 0.1)
	{
		_timerForFrame = TIMEMANAGER->getWorldTime();
		frameUpdate();
	}

	if (_dead) _alpha += 1;
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void bat::move()
{
	_pointx += _xspeed;
	_pointy += _yspeed;

	if (_xspeed > 0) _xspeed -= 0.1;
	else if (_xspeed < 0) _xspeed += 0.1;
	if (_yspeed > 0) _yspeed -= 0.1;
	else if (_yspeed < 0) _yspeed += 0.1;

	if (abs(_xspeed) <= 0.5) _xspeed = 0;
	if (abs(_yspeed) <= 0.5) _yspeed = 0;

	//속도 한계치
	if (_xspeed > 2) _xspeed = 2;
	else if (_xspeed < -2) _xspeed = -2;
	if (_yspeed > 2) _yspeed = 2;
	else if (_yspeed < -2) _yspeed = -2;
	
}

void bat::actByState()
{
	if (_dead)
	{
		_xspeed = 0;
		_yspeed = 3;
	}
	else
	{
		switch (_batstate)
		{
		case BATSTATE_FLYING:
			if (_isPlayerOnTarget)
			{
				if(abs(_xspeed) < 1)
				_xspeed += cosf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y))*_statistics.spd;
				if(abs(_yspeed) < 1)
				_yspeed += -sinf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y))*_statistics.spd;
			}
			else
			{
				_yspeed += -_statistics.spd;
			}
			break;
		case BATSTATE_HIT:
			if (TIMEMANAGER->getWorldTime() - _hittimer > 0.7) _batstate = BATSTATE_FLYING;
			break;
		case BATSTATE_SLEEP:
			if (_isPlayerOnTarget) _batstate = BATSTATE_FLYING;
			break;
		}
	}


}
void bat::hitPlayer()
{
	RECT temp;
	if (IntersectRect(&temp, &_player->getRect(), &_rc))
	{
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), _statistics.str);
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 10;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 10;
	}
}
void bat::mapCollisionCheck()
{
	mapInfo maptile;
	RECT temp;
	RECT smallrect;
	smallrect = RectMakeCenter(_pointx, _pointy, 4, 4);
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			maptile = _map->getMapInfo(int(_pointy) / TILESIZE - i, int(_pointx) / TILESIZE - j);
			if (maptile.type == MAPTILE_WALL || maptile.type == MAPTILE_WALL2)
			{
				if (IntersectRect(&temp, &maptile.rc, &smallrect))
				{
					_pointx += cosf(getAngle(maptile.point.x + TILESIZE / 2, maptile.point.y + TILESIZE / 2, _pointx, _pointy)) * 1;//(temp.right - temp.left + 2);
					_pointy -= sinf(getAngle(maptile.point.x + TILESIZE / 2, maptile.point.y + TILESIZE / 2, _pointx, _pointy)) * 1;//(temp.bottom - temp.top + 2);
					if (!_isPlayerOnTarget && _batstate != BATSTATE_SLEEP)
					{
						_batstate = BATSTATE_SLEEP;
					}
				}
			}
		}
	}

}

void bat::deadcheck()
{
	if (_statistics.hp <= 0)
	{
		_dead = true;
		if (_alpha >= 255)
		{
			_deleteForeEm = true;
		}
	}
}
void bat::imgHandleByState()
{
	switch (_batstate)
	{
	case BATSTATE_FLYING:
		_image = IMAGEMANAGER->findImage("batflying");
		break;
	case BATSTATE_HIT:
		_image = IMAGEMANAGER->findImage("bathit");
		break;
	case BATSTATE_SLEEP:
		_image = IMAGEMANAGER->findImage("batsleep");
		break;
	}
}
void bat::render(POINT camera)
{
	draw(camera);
}

void bat::draw(POINT camera)
{

	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	_image->alphaFrameRender(getMemDC(),
		_pointx - _image->getFrameWidth() / 2 + camera.x,
		_pointy - _image->getFrameHeight() / 2 + camera.y,
		_currentFrameX, _currentFrameY, 0);
}

void bat::frameUpdate()
{
	if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
	else _currentFrameX++;
}