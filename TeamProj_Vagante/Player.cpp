#include "stdafx.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"
#include "UI.h"

//그건인정

//흠..

//이야 머단해!!


Player::Player()
{
}


Player::~Player()
{
}
HRESULT Player::init(POINT point)
{
	//임의 설정이니 수정해주세요~
	_player.pointx = point.x;
	_player.pointy = point.y;
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 15, 20);

	_player.jumppower = 0;
	_player.lookingLeft = true;
	_player.lookingRight = false;
	_player.xspeed = 0;
	_player.yspeed = 0;
	_player.money = 0;
	_player.gravity = 0.3;
	for (int i = 0; i < 5; i++)_player.statusEffect[i].type = STATUSEFFECT_NULL;
	_player.stat.hp = 0;
	_player.stat.str = 0;
	_player.stat.dex = 0;
	_player.stat.vit = 0;
	_player.stat.inl = 0;
	_player.stat.lck = 0;
	_player.stat.def = 0;
	_player.stat.fir = 0;
	_player.stat.ice = 0;
	_player.stat.lgt = 0;
	_player.stat.psn = 0;
	_player.stat.mel = 0;
	_player.stat.rng = 0;
	_player.stat.crit = 0;
	_player.stat.aspd = 0;
	_player.stat.spd = 0;
	_canCtrl = true;
	return S_OK;
}
void Player::release()
{

}
void Player::update()
{
	//test~
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, TILESIZE / 2, TILESIZE * 3 / 4);
	//~test

	//움직임
	move();
	//타일값 읽기
	setmaptileInfo();
	//조작 가능하면 키 입력 받기 (ui에게 조작이 넘어갈 경우 _canCtrl값 false)
	if (_canCtrl) keyintput();
}
void Player::render()
{

}
//그릴 때	x좌표에 camera.x 만큼
//			y좌표에 camera.y 만큼 더해주기!!!!
void Player::render(POINT camera)
{
	draw(camera);
}
void Player::draw(POINT camera)
{
	//test
	Rectangle(getMemDC(), _player.rc.left + camera.x, _player.rc.top + camera.y, _player.rc.right + camera.x, _player.rc.bottom + camera.y);

}
void Player::move()
{
	//이동
	//_player.pointx += _player.xspeed;
	//_player.pointy -= _player.yspeed;


}

void Player::keyintput()
{
	//test~
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
	//~test

	//이동관련
	bool canMove = true;
	//스턴중인지 체크
	for (int i = 0; i < 5; i++)
	{
		if (_player.statusEffect[i].type == STATUSEFFECT_STUN) canMove = false;
	}
	//아니라면 행동
	if (canMove)
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
			_player.pointx -= _player.stat.spd;
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_player.state = PLAYERSTATE_MOVING;
			_player.lookingLeft = false;
			_player.lookingRight = true;
			_player.pointx += _player.stat.spd;
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
void Player::addStatusEffect(tagStatusEffect statuseffect)
{
	//상태이상 추가!
	for (int i = 0; i < 5; i++)
	{
		if (_player.statusEffect[i].type == NULL)
		{
			_player.statusEffect[i] = statuseffect;
			break;
		}
	}
}
void Player::setmaptileInfo()
{
	/*타일정보
	type

	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER, // 사다리
	MAPTILE_GROUND_CAN_GO_DOWN_1, // 발판
	MAPTILE_GROUND_CAN_GO_DOWN_2, // 발판
	MAPTILE_TRAP_SPIKE,
	MAPTILE_TRAP_ARROW,
	MAPTILE_TRAP_ROCK
	*/
	//캐릭터 기준으로 9개의 타일 정보 받아오기
	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;
	upL = _map->getMapInfo(int(_player.pointy) / TILESIZE - 1, int(_player.pointx) / TILESIZE - 1);
	upM = _map->getMapInfo(int(_player.pointy) / TILESIZE - 1, int(_player.pointx) / TILESIZE);
	upR = _map->getMapInfo(int(_player.pointy) / TILESIZE - 1, int(_player.pointx) / TILESIZE + 1);
	midL = _map->getMapInfo(int(_player.pointy) / TILESIZE, int(_player.pointx) / TILESIZE - 1);
	midM = _map->getMapInfo(int(_player.pointy) / TILESIZE, int(_player.pointx) / TILESIZE);
	midR = _map->getMapInfo(int(_player.pointy) / TILESIZE, int(_player.pointx) / TILESIZE + 1);
	botL = _map->getMapInfo(int(_player.pointy) / TILESIZE + 1, int(_player.pointx) / TILESIZE - 1);
	botM = _map->getMapInfo(int(_player.pointy) / TILESIZE + 1, int(_player.pointx) / TILESIZE);
	botR = _map->getMapInfo(int(_player.pointy) / TILESIZE + 1, int(_player.pointx) / TILESIZE + 1);


	//타일 활용 예시

	//플레이어 아래 타일 체크
	switch (botM.type)
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
			if (IntersectRect(&temp, &_player.rc, &botM.rc))
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