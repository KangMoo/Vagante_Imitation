#include "stdafx.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"

Player::Player()
{
}


Player::~Player()
{
}
HRESULT Player::init(POINT point)
{
	_player.pointx = point.x;
	_player.pointy = point.y;
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 15, 20);
	_player.hp = 150;
	_player.jumppower = 0;
	_player.lookingLeft = true;
	_player.lookingRight = false;
	_player.atkspeed = 1;
	_player.speed = 3;
	_player.xspeed = 0;
	_player.yspeed = 0;
	_player.money = 0;
	_player.gravity = 0.3;
	_player.statusEffect.type = STATUSEFFECT_IDLE;
	_canCtrl = true;
	return S_OK;
}
void Player::release()
{

}
void Player::update()
{
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 15, 30);
	move();
	//조작 가능하면 키 입력 받기
	if(_canCtrl) keyintput();
}
void Player::render()
{

}
//그릴 때	x좌표에 (WINSIZEX/2 - camera.x) 만큼
//			y좌표에 (WINSIZEY/2 - camera.y) 만큼 더해주기!!!!
void Player::render(POINT camera)
{
	draw(camera);
}
void Player::draw(POINT camera)
{
	//test
	Rectangle(getMemDC(), _player.rc.left + (WINSIZEX / 2 - camera.x), _player.rc.top + (WINSIZEY / 2 - camera.y), _player.rc.right + (WINSIZEX / 2 - camera.x), _player.rc.bottom + (WINSIZEY / 2 - camera.y));

}
void Player::move()
{
	//이동
	//_player.pointx += _player.xspeed;
	//_player.pointy -= _player.yspeed;

	collisionCheck();
}

void Player::keyintput()
{
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		_player.pointy -= 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		_player.pointy += 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		_player.pointx -= 2;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		_player.pointx += 2;
	}

	//이동관련
	if (_player.statusEffect.type != STATUSEFFECT_STUN)
	{
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			_player.state = PLAYERSTATE_LOOKING_UP;
		}
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			_player.state = PLAYERSTATE_LOOKING_DOWN;
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_player.state = PLAYERSTATE_MOVING;
			_player.lookingLeft = true;
			_player.lookingRight = false;
			_player.pointx -= _player.speed;
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_player.state = PLAYERSTATE_MOVING;
			_player.lookingLeft = false;
			_player.lookingRight = true;
			_player.pointx += _player.speed;
		}
		if (KEYMANAGER->isOnceKeyDown('Z'))
		{
			jump();
		}
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			attack();
		}
		if (KEYMANAGER->isOnceKeyDown('C'))
		{

		}
		if (KEYMANAGER->isOnceKeyDown('V'))
		{

		}
	}

}
void Player::jump()
{
	//추가
}
void Player::attack()
{
	//추가
}
void Player::collisionCheck()
{
	//캐릭터 기준으로 상,하,좌,우의 타일값 읽어들이기
	mapInfo uptile, lefttile, righttile, downtile;
	uptile = _map->getMapInfo(int(_player.pointy) % TILESIZE - 1, int(_player.pointx) % TILESIZE);
	downtile = _map->getMapInfo(int(_player.pointy) % TILESIZE + 1, int(_player.pointx) % TILESIZE);
	lefttile = _map->getMapInfo(int(_player.pointy) % TILESIZE, int(_player.pointx) % TILESIZE - 1);
	righttile = _map->getMapInfo(int(_player.pointy) % TILESIZE, int(_player.pointx) % TILESIZE + 1);


	//플레이어 아래 타일 체크
	switch (downtile.type)
	{
		//아무것도 없을경우
	case MAPTILE_NULL:
		break;
		//벽&땅일경우
	case MAPTILE_WALL: case MAPTILE_WALL2:
		for (int i = 0; i < _player.yspeed; i++)
		{
			RECT temp;
			//바닥에 착지 할 경우
			if (IntersectRect(&temp, &_player.rc, &downtile.rc))
			{
				//추가
				
			}
		}
		break;
		//사다리일경우
	case MAPTILE_LADDER:
		break;
		//발판일경우
	case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
		break;
		//함정일경우 (맵에서 처리)
	case MAPTILE_TRAP_SPIKE:case MAPTILE_TRAP_ARROW:case MAPTILE_TRAP_ROCK:
		break;
	}

	/*
	switch (downtile.type)
	{
	case MAPTILE_NULL:
		break;
	case MAPTILE_WALL:
		break;
	case MAPTILE_WALL2:
		break;
	case MAPTILE_LADDER:
		break;
	case MAPTILE_GROUND_CAN_GO_DOWN_1:
		break;
	case MAPTILE_GROUND_CAN_GO_DOWN_2:
		break;
	case MAPTILE_TRAP_SPIKE:
		break;
	case MAPTILE_TRAP_ARROW:
		break;
	case MAPTILE_TRAP_ROCK:
		break;
	}
	*/
}