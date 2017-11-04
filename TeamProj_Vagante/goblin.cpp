#include "stdafx.h"
#include "goblin.h"
#include "Player.h"

goblin::goblin()
{
}

goblin::~goblin()
{
}

HRESULT goblin::init(POINT point, float minCog, float maxCog)
{
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = RND->getFromIntTo(0, 5);
	_isFindPlayer = false;
	_attack = false;

	idleImg = new image;
	moveImg = new image;
	attackImg = new image;
	attackImgLeft = new image;
	attackImgRight = new image;
	jumpImg = new image;
	hitImg = new image;
	deadImg = new image;

	//IMAGEMANAGER->addFrameImage("goblin_Idle", "image\\enemy\\goblin_idle.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("doblin_Hit", "image\\enemy\\goblin_hit.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Dead", "image\\enemy\\goblin_dead.bmp", 32, 68, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Attack", "image\\enemy\\goblin_attack.bmp,", 256, 68, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Jump", "image\\enemy\\goblin_jump.bmp", 128, 68, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("goblin_Move", "image\\enemy\\goblin_move.bmp,", 256, 68, 8, 2, true, RGB(255, 0, 255));

	idleImg->init("Img\\enemy\\goblin_idle.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));
	//moveImg->init("img\\enemy\\goblin_move.bmp", 256, 64, 8, 2, true, RGB(255, 0, 255));

	attackImg->init("Img\\enemy\\goblin_attack.bmp", 256, 64, 4, 2, true, RGB(255, 0, 255));

	//attackImgLeft->init("Img\\enemy\\goblin_attack_left.bmp", 256, 64, 4, 1, true, RGB(255, 0, 255));
	//attackImgRight->init("Img\\enemy\\goblin_attack_right.bmp", 256, 64, 4, 1, true, RGB(255, 0, 255));
	//attackImgLeft->init("goblin_attack_left.bmp", 256, 64, 4, 1, true, RGB(255, 0, 255));
	//attackImgRight->init("goblin_attack_right.bmp", 256, 64, 4, 1, true, RGB(255, 0, 255));

	jumpImg->init("Img\\enemy\\goblin_jump.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));
	hitImg->init("Img\\enemy\\goblin_hit.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));
	deadImg->init("Img\\enemy\\goblin_dead.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));

	moveImg->init("Img\\enemy\\goblin_moving.bmp", 256, 64, 8, 2, true, RGB(255, 0, 255));

	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);

	_findRange = minCog;
	_findRangeMax = maxCog;

	//_findPlayerRect = RectMakeCenter(_pointx + 16, _pointy + 16, (int)_findRange, (int)_findRange);

	//_findRectRange = RectMakeCenter(_pointx + 16, _pointy + 16, _findRange, _findRange); //맨이터에서 가져온것이니 삭제해도 무방합니다.

	_statistics.hp = 16;
	_statistics.str = 1;
	_statistics.dex = 0;
	_statistics.vit = 0;
	_statistics.inl = 0;
	_statistics.lck = 0;
	_statistics.def = 0;
	_statistics.fir = 0;
	_statistics.ice = 0;
	_statistics.lgt = 0;
	_statistics.psn = 0;
	_statistics.mel = 0;
	_statistics.rng = 0;
	_statistics.crit = 0;
	_statistics.aspd = 0;
	_statistics.spd = 0;

	_xspeed = _yspeed = 0;

	_image = idleImg;

	_currentFrameX = 1;
	_currentFrameY = 0;

	_frameFPS = 10;
	_frameTime = 0;

	_dead = false;

	_deadAlpha = 0;

	_isOnLand = false;

	_jumptimer = TIMEMANAGER->getWorldTime();

	_gravity = 0;
	return S_OK;
}

void goblin::release()
{

}

void goblin::update()
{
	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 200) _isFindPlayer = true;
	else _isFindPlayer = false;

	mapCollisionCheck();
	attRectClear();

	frameUpdate();

	if (KEYMANAGER->isOnceKeyDown('N'))
	{
		getDamaged(1);
		_image = hitImg;
	}

	if (PtInRect(&_findPlayerRect, _player->getPoint()) && _statistics.hp >= 0);
	{
		move();
		//jump();
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack();
	}

	if (_statistics.hp <= 0)//사망체크
	{
		_image = deadImg;
		_state = ENEMYSTATE_DEAD;
	}

	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha > 255)
		{
			_dead = true;
		}
	}

	//if (_state == ENEMYSTATE_HIT)
	//{
	//	_image = hitImg;
	//}
}

void goblin::render(POINT camera)
{
	draw(camera);
}

void goblin::draw(POINT camera)
{
	Rectangle(getMemDC(), _findPlayerRect.left, _findPlayerRect.top, _findPlayerRect.right, _findPlayerRect.bottom);
	_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, _currentFrameY);
	//if (right == false && _state == ENEMYSTATE_ATTACKING)
	//{
	//	_image->frameRender(getMemDC(), _rc.left - 32 + camera.x, _rc.top + camera.y, _currentFrameX, 1);
	//}
}

void goblin::move()
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

	_state = ENEMYSTATE_MOVING;

	_image = moveImg;

	if (_pointx / TILESIZE > _player->getPoint().x / TILESIZE) //플레이어 기준 플레이어보다 오른쪽에 있을때 왼쪽을 바라보게 함
	{
		_right = false;
		//_xspeed -= _statistics.spd;
		_xspeed -= 0.5;
	}
	else if ((_pointx / TILESIZE < _player->getPoint().x / TILESIZE))
	{
		_right = true;
		//_xspeed += _statistics.spd;
		_xspeed += 0.5;

	} //플레이어 기준 플레이어보다 왼쪽에 있을때 오른쪽을 바라보게 함

}

void goblin::jump()
{
	_image = jumpImg;

	if (TIMEMANAGER->getWorldTime() - _jumptimer > 1)
	{
		_yspeed -= 10;
		_state = ENEMYSTATE_JUMPING;
		_jumptimer = TIMEMANAGER->getWorldTime();
	}

	if (_state == ENEMYSTATE_JUMPING || _state == ENEMYSTATE_FALLING)
	{
		if (_yspeed > 0) _state = ENEMYSTATE_FALLING;

		if (_pointx > _player->getPoint().x) //플레이어 기준 플레이어보다 오른쪽에 있을때 왼쪽을 바라보게 함
		{
			_right = false;
			_xspeed -= _statistics.spd;
			_currentFrameY = 0;
		}
		else //플레이어 기준 플레이어보다 왼쪽에 있을때 오른쪽을 바라보게 함
		{
			_right = true;
			_xspeed += _statistics.spd;
			_currentFrameY = 1;
		}
	}
	if (_isOnLand)  _state = ENEMYSTATE_IDLE;
	//
}

void goblin::attack()
{

	_state = ENEMYSTATE_ATTACKING;

	_statistics.mel = RND->getFromIntTo(6, 7);

	if (_isFindPlayer && _statistics.hp >= 0)
	{
		_image = attackImg;

		if (_right == true && _player->getPoint().x <= _pointx + 32)
		{
			//_image = attackImgRight;
			_attackRect = RectMake(_pointx + 32, _pointy + 8, 32, 16);
		}

		if (_right == false && _player->getPoint().x >= _pointx - 32)
		{
			//_image = attackImgLeft;
			_attackRect = RectMake(_pointx, _pointy + 8, 32, 16);
		}
	}

	RECT temp;
	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //플레이어를 공격했다.
	{
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), _statistics.mel);
		//플레이어 반대방향으로 튕겨나기
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
	}

}

void goblin::falling()
{

}

void goblin::frameUpdate()
{
	_frameFPS = 10;
	_frameTime++;
	if (_frameFPS <= _frameTime)
	{
		_currentFrameX++;
		_frameTime = 0;
	}

	if (_currentFrameX > _image->getMaxFrameX())
	{
		_currentFrameX = 0;
	}

	if (_right == true && _state != ENEMYSTATE_ATTACKING) _currentFrameY = 0;
	else if (_right == false && _state != ENEMYSTATE_ATTACKING) _currentFrameY = 1;
}

void goblin::playerCog()
{

}

void goblin::mapCollisionCheck()
{
	_upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	_upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	_upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	_midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	_midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	_midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	_botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	_botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	_botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);

	_rc = RectMakeCenter(_pointx, _pointy, TILESIZE, TILESIZE);
	RECT temp;
	if ((_midL.type == MAPTILE_WALL || _midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &_midL.rc, &_rc))
	{
		_pointx += temp.right - temp.left;
		jump();
	}
	if ((_midR.type == MAPTILE_WALL || _midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &_midR.rc, &_rc))
	{
		_pointx -= temp.right - temp.left;
		jump();
	}
	if ((_upM.type == MAPTILE_WALL || _upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &_upM.rc, &_rc))
	{
		_pointy += temp.bottom - temp.top;
	}

	if (_player->getPoint().y >= _pointy)
	{
		if ((_botM.type == MAPTILE_WALL || _botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &_botM.rc, &_rc))
		{
			_pointy -= temp.bottom - temp.top;
			_yspeed = 0;
			_isOnLand = true;
		}
	}
	else
	{
		if ((_botM.type == MAPTILE_WALL || _botM.type == MAPTILE_WALL2 || _botM.type == MAPTILE_GROUND_CAN_GO_DOWN_1) && IntersectRect(&temp, &_botM.rc, &_rc))
		{
			_pointy -= temp.bottom - temp.top;
			_yspeed = 0;
			_isOnLand = true;
		}
	}
	_rc = RectMakeCenter(_pointx, _pointy, TILESIZE, TILESIZE);

}