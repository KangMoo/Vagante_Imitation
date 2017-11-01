#include "stdafx.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"
#include "UI.h"

// Created by YOOJOOWON 17.11

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

	_player.state = PLAYERSTATE_FALLING;

	_curTileX = _prevTileX = int(_player.pointx) / TILESIZE;
	_curTileY = _prevTileY = int(_player.pointy) / TILESIZE;

	upL = _map->getMapInfo(_curTileY - 1, _curTileX - 1);
	upM = _map->getMapInfo(_curTileY - 1, _curTileX);
	upR = _map->getMapInfo(_curTileY - 1, _curTileX + 1);
	midL = _map->getMapInfo(_curTileY, _curTileX - 1);
	midM = _map->getMapInfo(_curTileY, _curTileX);
	midR = _map->getMapInfo(_curTileY, _curTileX + 1);
	botL = _map->getMapInfo(_curTileY + 1, _curTileX - 1);
	botM = _map->getMapInfo(_curTileY + 1, _curTileX);
	botR = _map->getMapInfo(_curTileY + 1, _curTileX + 1);

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
	_player.pointx += _player.xspeed;
	_player.pointy -= _player.yspeed;
}

void Player::keyintput()
{
	/*
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
	*/
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

		//방향 설정
		if (_player.state != PLAYERSTATE_HOLDING_WALL) {
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
				_player.lookingRight = true;
			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
				_player.lookingRight = false;
		}
				
		switch (_player.state) {
		case PLAYERSTATE_IDLE:
			//올려다보는 모션
			if (KEYMANAGER->isStayKeyDown(VK_UP))
				_player.currentFrameX = 1;
			if (KEYMANAGER->isOnceKeyUp(VK_UP))
				_player.currentFrameX = 0;
			

			//이동
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.currentFrameX = 0;
				_player.xspeed = RUNPOWERSTART;
				_player.state = PLAYERSTATE_MOVING;
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.currentFrameX = 0;
				_player.xspeed = -RUNPOWERSTART;
				_player.state = PLAYERSTATE_MOVING;
			}

			//점프
			if (KEYMANAGER->isOnceKeyDown('X')) {
				_player.currentFrameX = 0;
				_player.yspeed = JUMPPOWERSTART;
				_player.state = PLAYERSTATE_JUMPING;
			}

			ladder();
			canDown();

			break;
		case PLAYERSTATE_LOOKING_UP:
			break;
		case PLAYERSTATE_LOOKING_DOWN:
			break;

		case PLAYERSTATE_MOVING:

			// 스톱
			if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_player.lookingRight) {
				_player.currentFrameX = 0;
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _player.lookingRight) {
				_player.currentFrameX = 0;
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
			}


			// 가속
			if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed -= 1;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed += 1;
			}
			if (_player.xspeed > RUNPOWERMAX)
				_player.xspeed = RUNPOWERMAX;
			if (_player.xspeed < -RUNPOWERMAX)
				_player.xspeed = -RUNPOWERMAX;


			// 점프
			if (KEYMANAGER->isOnceKeyDown('X')) {
				_player.currentFrameX = 0;
				_player.yspeed = JUMPPOWERSTART;
				_player.state = PLAYERSTATE_JUMPING;
			}


			ladder();
			canDown();
			
			break;
		case PLAYERSTATE_JUMPING:
			// 중력
			_player.yspeed -= 0.5;

			// 꾹 누르면 더 높이 올라감
			if (KEYMANAGER->isStayKeyDown('X')) {
				_player.yspeed += 0.2;
			}

			// 점프 중 좌우 이동
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed += 0.5;
				if (_player.xspeed > RUNPOWERMAX)
					_player.xspeed = RUNPOWERMAX;
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed -= 0.5;
				if (_player.xspeed < -RUNPOWERMAX)
					_player.xspeed = -RUNPOWERMAX;
			}

			// 좌우 키보드 뗄 시 수직 강하로 바뀜 (포물선처럼 보이게)
			if (_player.xspeed > 0)
				_player.xspeed -= 0.2;
			else if (_player.xspeed < 0)
				_player.xspeed += 0.2;


			//FALLING 상태로 변경
			if (_player.yspeed < 0)
				_player.state = PLAYERSTATE_FALLING;
			

			ladder();
			
			break;

		case PLAYERSTATE_FALLING:
			// 중력 (최대 속도 제한 있음)
			if (_player.yspeed > -FALLPOWERMAX)
				_player.yspeed -= 0.5;
			else
				_player.yspeed = -FALLPOWERMAX;
			

			// 점프 중 좌우 이동
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed += 0.5;
				if (_player.xspeed > RUNPOWERMAX)
					_player.xspeed = RUNPOWERMAX;
			}

			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed -= 0.5;
				if (_player.xspeed < -RUNPOWERMAX)
					_player.xspeed = -RUNPOWERMAX;
			}

			// 좌우 키보드 뗄 시 수직 강하로 바뀜 (포물선처럼 보이게)
			if (_player.xspeed > 0)
				_player.xspeed -= 0.2;
			else if (_player.xspeed < 0)
				_player.xspeed += 0.2;


			ladder();
			break;
			

		case PLAYERSTATE_ATTACKING_1:

			break;
		case PLAYERSTATE_ATTACKING_2:

			break;

		case PLAYERSTATE_ATTACKING_3:

			break;
		case PLAYERSTATE_HIT:

			break;
		case PLAYERSTATE_HOLDING_WALL:		
			
			_player.xspeed = 0;

			if (KEYMANAGER->isOnceKeyDown('X')) {
				_player.yspeed = JUMPPOWERSTART * 0.8;
				if (_player.lookingRight) {
					_player.pointx -= 1;
					_player.xspeed = RUNPOWERSTART;
				}
				else {
					_player.pointx += 1;
					_player.xspeed = -RUNPOWERSTART;
				}
				_player.state = PLAYERSTATE_JUMPING;
			}


			if (_player.lookingRight)
			{
				if (KEYMANAGER->isOnceKeyDown(VK_LEFT)) {
					_player.pointx -= 1;
					_player.yspeed = -3;
					_player.xspeed = -RUNPOWERSTART;
					_player.state = PLAYERSTATE_FALLING;

					_player.lookingRight = false;
				}
			}
			else
			{
				if (KEYMANAGER->isOnceKeyDown(VK_RIGHT)) {
					_player.pointx += 1;
					_player.yspeed = -3;
					_player.xspeed = RUNPOWERSTART;
					_player.state = PLAYERSTATE_FALLING;

					_player.lookingRight = true;
				}
			}

			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) {
				_player.yspeed = -5;
				if (_player.lookingRight) {
					_player.pointx -= 2;
					_player.xspeed = -RUNPOWERSTART * 0.2;
				}
				else
				{
					_player.pointx += 2;
					_player.xspeed = +RUNPOWERSTART * 0.2;
				}
				_player.state = PLAYERSTATE_FALLING;
			}

			break;

		case PLAYERSTATE_HOLDING_LADDER:
			if (KEYMANAGER->isStayKeyDown(VK_UP) || KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				if (KEYMANAGER->isStayKeyDown(VK_UP) && upM.type == MAPTILE_LADDER)
					_player.yspeed = 5;
				if (KEYMANAGER->isStayKeyDown(VK_DOWN) && botM.type == MAPTILE_LADDER)
					_player.yspeed = -5;
			}
			else
				_player.yspeed = 0;

			break;
		case PLAYERSTATE_CHEKINGINVENTORY:

			break;
		}
	}

	/*
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
	*/

}
void Player::jump()
{
	//추가
}
void Player::attack()
{
	//추가
}

void Player::ladder()
{
	if (midM.type == MAPTILE_LADDER && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_player.pointx = midM.rc.left + (midM.rc.right - midM.rc.left) * 0.5;
		_player.state = PLAYERSTATE_HOLDING_LADDER;
		_player.xspeed = 0;
		_player.yspeed = 0;
		_player.currentFrameX = 0;
	}

}

void Player::canDown()
{
	if ((botM.type == MAPTILE_GROUND_CAN_GO_DOWN_1)
		&& KEYMANAGER->isStayKeyDown(VK_DOWN) && KEYMANAGER->isStayKeyDown('X')) {
		_player.state = PLAYERSTATE_FALLING;
		_player.yspeed = 0;
		_player.pointy = botM.rc.top + (_player.rc.bottom - _player.rc.top) * 0.25;
	}


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
	_curTileX = int(_player.pointx) / TILESIZE;
	_curTileY = int(_player.pointy) / TILESIZE;

	if (_curTileX != _prevTileX || _curTileY != _prevTileY) {
		upL = _map->getMapInfo(_curTileY - 1, _curTileX - 1);
		upM = _map->getMapInfo(_curTileY - 1, _curTileX);
		upR = _map->getMapInfo(_curTileY - 1, _curTileX + 1);
		midL = _map->getMapInfo(_curTileY, _curTileX - 1);
		midM = _map->getMapInfo(_curTileY, _curTileX);
		midR = _map->getMapInfo(_curTileY, _curTileX + 1);
		botL = _map->getMapInfo(_curTileY + 1, _curTileX - 1);
		botM = _map->getMapInfo(_curTileY + 1, _curTileX);
		botR = _map->getMapInfo(_curTileY + 1, _curTileX + 1);

		_prevTileX = _curTileX;
		_prevTileY = _curTileY;

	}

	//타일 활용 예시

	//플레이어 아래 타일 체크

	//midM
	//1. (속도 문제 등으로) 벽에 끼일 경우 쫓아내야 한다

	switch (midM.type) {

	//벽일 경우, 별 조건 없이 쫓아내도 된다
	case MAPTILE_WALL: case MAPTILE_WALL2: 

		//falling이라면 아래로 부딪힌 상황일테니 위로 보내고 idle로 만든다
		if (_player.state == PLAYERSTATE_FALLING) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.currentFrameX = 8;
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_IDLE;
		}

		//jump라면 위로 부딪힌 상황일테니 밑으로 튕겨낸다
		if (_player.state == PLAYERSTATE_JUMPING) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -3;	
			_player.pointy = midM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
		}
	break;

	//내려갈 수 있는 발판의 경우 밑으로 튕겨낼 필요는 없다
	
	case MAPTILE_GROUND_CAN_GO_DOWN_1:
		//falling일 경우 아래로 부딪힌 상황이라면 위로 보내고 idle로 만든다
		//하지만 아래에서 접근하면서도 falling 상태일 수도 있다. 이건 플레이어 위치와 속도로 예외처리
		if (_player.state == PLAYERSTATE_FALLING 
			&& _player.yspeed < -FALLPOWERMAX * 0.5) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5f);
			_player.currentFrameX = 8;
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_IDLE;
		}
	break;
	}

	switch (botM.type)
	{
		//아무것도 없을경우
	case MAPTILE_NULL:	case MAPTILE_LADDER:
		if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -3;
		}
	break;
		//벽&땅일경우
	case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1:
		//바닥에 착지 할 경우
		if (isCollision(_player.rc, botM.rc))
		{
			if (_player.state == PLAYERSTATE_FALLING) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
				_player.currentFrameX = 8;
				_player.xspeed = 0;
				_player.yspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
			}
			//추가			
		}
	break;
		//사다리일경우
	}


	/*
	//botR, botL
	//대각선 밑에 걸치는 상황 고려

	//버벅거려서 주석 처리함

	if (_player.state != PLAYERSTATE_IDLE && _player.state != PLAYERSTATE_MOVING) {
		switch (botR.type)
		{
			//아무것도 없을경우
		case MAPTILE_NULL:	case MAPTILE_LADDER:
			if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = -3;
			}
			break;
			//벽&땅일경우
		case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
			//바닥에 착지 할 경우
			if (isCollision(_player.rc, botR.rc))
			{
				if (_player.state == PLAYERSTATE_FALLING) {
					_player.pointy = botR.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
					_player.currentFrameX = 8;
					_player.xspeed = 0;
					_player.yspeed = 0;
					_player.state = PLAYERSTATE_IDLE;
				}
				//추가			
			}
			break;
			//사다리일경우
		}

		switch (botL.type)
		{
			//아무것도 없을경우
		case MAPTILE_NULL:	case MAPTILE_LADDER:
			if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = -3;
			}
			break;
			//벽&땅일경우
		case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
			//바닥에 착지 할 경우
			if (isCollision(_player.rc, botL.rc))
			{
				if (_player.state == PLAYERSTATE_FALLING) {
					_player.pointy = botL.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
					_player.currentFrameX = 8;
					_player.xspeed = 0;
					_player.yspeed = 0;
					_player.state = PLAYERSTATE_IDLE;
				}
				//추가			
			}
			break;
			//사다리일경우
		}
	}

	*/
	switch (midR.type) {

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (_player.rc.right > midR.rc.left) {
			_player.pointx = midR.rc.left - ((_player.rc.right - _player.rc.left) * 0.5);
			_player.xspeed = 0;

			
			if ((upR.type != MAPTILE_WALL && upR.type != MAPTILE_WALL2) &&
				(_player.rc.top > midR.rc.top - 10 && _player.rc.top < midR.rc.top))
			{
				_player.pointy = midR.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5;
				_player.state = PLAYERSTATE_HOLDING_WALL;
				_player.xspeed = 0;
				_player.yspeed = 0;
			}
		}
		break;
	}


	switch (midL.type) {
	

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (_player.rc.left < midL.rc.right) {
			_player.pointx = midL.rc.right + ((_player.rc.right - _player.rc.left) * 0.5);
			_player.xspeed = 0;
			if ((upL.type != MAPTILE_WALL && upL.type != MAPTILE_WALL2)
				&& (_player.rc.top > midL.rc.top - 10 && _player.rc.top < midL.rc.top))
			{
				_player.pointy = midL.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5;
				_player.state = PLAYERSTATE_HOLDING_WALL;
				_player.xspeed = 0;
				_player.yspeed = 0;
			}
		}
		break;	
	}

	switch (upM.type) {
	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (isCollision(_player.rc, upM.rc)) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -2;
			_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			
		}

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