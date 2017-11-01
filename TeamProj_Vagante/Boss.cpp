#include "stdafx.h"
#include "Boss.h"
#include "Player.h"
#include "UI.h"
#include "FireBall.h"

Boss::Boss()
{
}


Boss::~Boss()
{
}


HRESULT Boss::init()
{
	return S_OK;
}
HRESULT Boss::init(POINT point)
{
	//초기화해주세욤
	_image = IMAGEMANAGER->findImage("boss_dragon_sleep");
	_state = BOSSSTATE_SLEEP;													//상태
	for (int i = 0; i < STATUSEFFECTMAX; i++)
	{
		_statusEffect[i].type = STATUSEFFECT_NULL;  //상태이상
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].damage = 0;
	}
	_currentFrameX = _currentFrameY = 0;									//프레임
	_pointx = point.x;
	_pointy = point.y;										//좌표
	_xspeed = _yspeed = 0;												//x,y축 이동 속도
	_money = 10;															//몬스터 죽으면 나올 동전 갯수
														   //맵 정보
	_isFindPlayer = false;													//플레이어를 발견한 상태인지
	_fireball = new FireBall;
	_fireball->setMapAddressLink(_map);
	_fireball->setPlayerAddressLink(_player);
	_fireball->init(100, 1000, "fireball");
	_actTimer = TIMEMANAGER->getWorldTime();
	//_frameTime, _frameFPS;											//프레임 변화용
	_minCog = 150;
	_maxCog = 1500;
	_statistics.hp = 100;
	_statistics.str = 5;
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
	_statistics.aspd = 1;
	_statistics.spd = 1;
	_canfire = true;
	_timerForFrameUpdate = TIMEMANAGER->getWorldTime();

	upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);
	_lookleft = true;
	_rc = RectMakeCenter(_pointx, _pointy, 40, 40);

	return S_OK;
}
void Boss::release()
{

}
void Boss::update()
{

	stateHandle();
	speedAdjust();
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
	move();
	if (TIMEMANAGER->getWorldTime() - _timerForFrameUpdate > 0.2)
	{
		frameUpdate();
		_timerForFrameUpdate = TIMEMANAGER->getWorldTime();
	}
	_fireball->update();
	
}
void Boss::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

//그릴 때	x좌표에 camera.x 만큼
//			y좌표에 camera.y 만큼 더해주기!!!!
void Boss::render(POINT camera)
{
	draw(camera);
}
void Boss::draw(POINT camera)
{

	//if((_pointx > camera.x && _pointx < camera.x + WINSIZEX) &&
	//	(_pointy > camera.y && _pointy < camera.y + WINSIZEY))
	//	_image->frameRender(getMemDC(), _rc.left - camera.x, _rc.top - camera.y);
	Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	_fireball->render(camera);
	_image->frameRender(getMemDC(), _pointx - _image->getFrameWidth()/2 + camera.x, _pointy - _image->getFrameHeight()/2 + camera.y, _currentFrameX, _currentFrameY);



}
void Boss::speedAdjust()
{

}
void Boss::move()
{
	if (_state == BOSSSTATE_SLEEP || _state == BOSSSTATE_DEAD)
	{
		_yspeed = -3;
	}
	_pointx += _xspeed;
	_pointy -= _yspeed;
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
	mapCollisionHandle();
	_xspeed = 0;
	_yspeed = 0;
}
void Boss::mapCollisionHandle()
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


	if ((upL.type == MAPTILE_WALL || upL.type == MAPTILE_WALL2) && isCollisionReaction(upL.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && isCollisionReaction(upM.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((upR.type == MAPTILE_WALL || upR.type == MAPTILE_WALL2) && isCollisionReaction(upR.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && isCollisionReaction(midL.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((midM.type == MAPTILE_WALL || midM.type == MAPTILE_WALL2) && isCollisionReaction(midM.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && isCollisionReaction(midR.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((botL.type == MAPTILE_WALL || botL.type == MAPTILE_WALL2) && isCollisionReaction(botL.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && isCollisionReaction(botM.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	else if ((botR.type == MAPTILE_WALL || botR.type == MAPTILE_WALL2) && isCollisionReaction(botR.rc, _rc))
	{
		_pointy = _rc.top + (_rc.bottom - _rc.top) / 2;
		_pointx = _rc.left + (_rc.right - _rc.left) / 2;
	}
	_rc = RectMakeCenter(_pointx, _pointy, 30, 30);
}
void Boss::addStatusEffect(tagStatusEffect statuseffect)
{
	//상태이상 추가!
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL)
		{
			_statusEffect[i] = statuseffect;
			break;
		}
	}
}

//상태이상 처리
void Boss::statusEffect()
{
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

void Boss::frameUpdate()
{
	if (_state != BOSSSTATE_SLEEP && _state != BOSSSTATE_DEAD)
	{
		if (_player->getPoint().x > _pointx)
		{
			_currentFrameY = 0;
		}
		else
		{
			_currentFrameY = 1;
		}
	}
	
	switch (_state)
	{
	case BOSSSTATE_SLEEP:
		
		break;
	case BOSSSTATE_ACTIVATE:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_FLYING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_FIREING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_STAMPING:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	case BOSSSTATE_DEAD:
		if (_currentFrameX >= _image->getMaxFrameX()) _currentFrameX = 0;
		else _currentFrameX++;
		break;
	}
}
void Boss::imageChange()
{
	switch (_state)
	{
	case BOSSSTATE_SLEEP:
		_image = IMAGEMANAGER->findImage("boss_dragon_sleep");
		break;
	case BOSSSTATE_ACTIVATE:
		_image = IMAGEMANAGER->findImage("boss_dragon_cry");
		break;
	case BOSSSTATE_FLYING:
		_image = IMAGEMANAGER->findImage("boss_dragon_fly");
		break;
	case BOSSSTATE_FIREING:
		_image = IMAGEMANAGER->findImage("boss_dragon_fire");
		break;
	case BOSSSTATE_STAMPING:
		_image = IMAGEMANAGER->findImage("boss_dragon_stamp");
		break;
	case BOSSSTATE_DEAD:
		_image = IMAGEMANAGER->findImage("boss_dragon_dead");
		break;
	}
	_currentFrameX = 0;
	_currentFrameY = 0;
}
void Boss::stateHandle()
{
	switch (_state)
	{
	case BOSSSTATE_SLEEP:
		//boss 활성화
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			//상태변경
			_state = BOSSSTATE_ACTIVATE;
			//상태 변화에 따른 이미지 변화
			imageChange();
			//시간 설정
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		break;
	case BOSSSTATE_ACTIVATE:
		//잠에서 깨어났을 때 (위로 조금 날아오른 뒤 BOSSSTATE_FLYING 으로 상태 변화)
		if (TIMEMANAGER->getWorldTime() - _actTimer > 2)
		{
			_state = BOSSSTATE_FLYING;
			//상태 변화에 따른 이미지 변화
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			_yspeed = _statistics.spd;
		}
		break;
	case BOSSSTATE_FLYING:
		//플레이어 방향으로 이동 후 일정 거리가 되면 BOSSSTATE_FIREING로 상태 변화

		if (TIMEMANAGER->getWorldTime() - _actTimer > 4 && getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 500)
		{
			_state = BOSSSTATE_FIREING;
			//상태 변화에 따른 이미지 변화
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			_xspeed = cosf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y)) * _statistics.spd;
			_yspeed = sinf(getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y)) * _statistics.spd;
		}
		break;
	case BOSSSTATE_FIREING:
		if (TIMEMANAGER->getWorldTime() - _actTimer > 5)
		{
			_state = BOSSSTATE_FLYING;
			//상태 변화에 따른 이미지 변화
			imageChange();
			_actTimer = TIMEMANAGER->getWorldTime();
		}
		else
		{
			fireFireBall();
		}
		break;
	case BOSSSTATE_STAMPING:

		break;
	case BOSSSTATE_DEAD:

		break;
	}
}

void Boss::fireFireBall()
{
	if (_canfire && _currentFrameX == 4)
	{
		_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 2.5);
		_canfire = false;
	}
	if (_currentFrameX == 0) _canfire = true;
}