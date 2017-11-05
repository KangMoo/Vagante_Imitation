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
	_invincible = false;
	_invincibleTime = 0;
	_test = 0;

	//이미지 추가 (나중에 main으로 옮길 예정)

	IMAGEMANAGER->addFrameImage("player_idle", "Img/player/player_idle.bmp", 96, 96, 2, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_jumping", "Img/player/player_jumping.bmp", 192, 96, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_moving", "Img/player/player_moving.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_ladderup", "Img/player/player_ladderup.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_ladderdown", "Img/player/player_ladderdown.bmp", 48, 96, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_holdingwall", "Img/player/player_holdingwall.bmp", 48, 96, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_attacking", "Img/player/player_attacking.bmp", 192, 96, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_lookingdown", "Img/player/player_lookingdown.bmp", 48, 96, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_lookingdownmoving", "Img/player/player_lookingdownmoving.bmp", 384, 96, 8, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("player_down", "Img/player/player_down.bmp", 288, 96, 6, 2, true, RGB(255, 0, 255));


	//애니메이션과 스테이터스 초기 설정
	firstSettingStat();
	_player.image = IMAGEMANAGER->findImage("player_idle");
	_animSpeed = 5;
	_animCount = 0;
	_animDelay = 0;

	//플레이어 위치 및 RECT
	_player.pointx = point.x;
	_player.pointy = point.y;
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 24, 24);


	//맵 정보 설정
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

	_attackDelay = 0;

	return S_OK;
}
void Player::release()
{

}
void Player::update()
{
	//test~
	_player.rc = RectMakeCenter(_player.pointx, _player.pointy, 24, 24);


	//공격 임시

	
	checkInvincible();
	checkStatusEffect();
	checkHitEnemy();
	checkItemBox();
	checkDropItem();


	//무적 체크
	
	
	//mapcollision();	
	//enemyCollision


	//조작 가능하면 키 입력 받기 (ui에게 조작이 넘어갈 경우 _canCtrl값 false)
	if (_canCtrl) keyintput();
	//움직임
	move();
	//타일값 읽기
	setmaptileInfo();

	//프레임 업데이트
	frameUpdate();
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

	_player.image->frameRender(getMemDC(), _player.rc.left - 12 + camera.x, _player.rc.top - 24 + camera.y, _player.currentFrameX, _player.curretFrameY);

	//_player.image->frameRender(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, _player.image->getFrameX(), _player.image->getFrameY());

	if (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP)
	{
		Rectangle(getMemDC(), _equipWeaponRect.left + camera.x, _equipWeaponRect.top + camera.y, _equipWeaponRect.right + camera.x, _equipWeaponRect.bottom + camera.y);

		if (_equipWeapon.name != NAME_HAND)
			if (!_player.lookingRight)
				_equipWeapon.img0->frameRender(getMemDC(), _equipWeaponRect.left + camera.x, _equipWeaponRect.top + camera.y, 1, 0);
			else
				_equipWeapon.img0->frameRender(getMemDC(), _equipWeaponRect.left + camera.x, _equipWeaponRect.top + camera.y, 0, 0);


	}



	Rectangle(getMemDC(), 100, 100, 200, 200);
	char str1[256];
	char str2[256];
	char str3[256];
	char str4[256];
	char str5[256];
	sprintf(str1, "%d %d %d", upL.type, upM.type, upR.type);
	sprintf(str2, "%d %d %d", midL.type, midM.type, midR.type);
	sprintf(str3, "%d %d %d", botL.type, botM.type, botR.type);
	sprintf(str4, "%ld %ld", _map->getCoinBox(1)._openBox, _map->getCoinBox(1)._eventChk);
	sprintf(str5, "%d", _em->getEnemyVector().size());
	TextOut(getMemDC(), 120, 110, str1, strlen(str1));
	TextOut(getMemDC(), 120, 130, str2, strlen(str2));
	TextOut(getMemDC(), 120, 150, str3, strlen(str3));
	TextOut(getMemDC(), 120, 170, str4, strlen(str4));
	TextOut(getMemDC(), 120, 190, str5, strlen(str5));
}


void Player::setStateImg() {

	_player.currentFrameX = 0;

	switch (_player.state)
	{
	case PLAYERSTATE_IDLE:
		_player.image = IMAGEMANAGER->findImage("player_idle");
		break;
	case PLAYERSTATE_LOOKING_DOWN:
		_player.image = IMAGEMANAGER->findImage("player_lookingdown");
		break;
	case PLAYERSTATE_LOOKING_DOWN_MOVING:
		_player.image = IMAGEMANAGER->findImage("player_lookingdownmoving");
		break;
	case PLAYERSTATE_MOVING:
		_player.image = IMAGEMANAGER->findImage("player_moving");
		break;
	case PLAYERSTATE_JUMPING:
	case PLAYERSTATE_FALLING:
		_player.image = IMAGEMANAGER->findImage("player_jumping");
		break;
	case PLAYERSTATE_ATTACKING:
	case PLAYERSTATE_ATTACKING_JUMP:
	case PLAYERSTATE_ATTACKING_3:
		_player.image = IMAGEMANAGER->findImage("player_attacking");
		_animDelay = 0;
		break;
	case PLAYERSTATE_HIT:
		_player.image = IMAGEMANAGER->findImage("player_idle");
		break;
	case PLAYERSTATE_HOLDING_WALL:
		_player.image = IMAGEMANAGER->findImage("player_holdingwall");
		break;
	case PLAYERSTATE_HOLDING_LADDERUP:
		_player.image = IMAGEMANAGER->findImage("player_ladderup");
		break;
	case PLAYERSTATE_HOLDING_LADDERDOWN:
		_player.image = IMAGEMANAGER->findImage("player_ladderdown");
		break;
	case PLAYERSTATE_CHEKINGINVENTORY:
		_player.image = IMAGEMANAGER->findImage("player_idle");
		break;
	case PLAYERSTATE_DEAD:
		_player.image = IMAGEMANAGER->findImage("player_down");
		break;
	}
}

void Player::frameUpdate() {
	_animCount += TIMEMANAGER->getElapsedTime() * 75;

	if (_player.lookingRight)
		_player.curretFrameY = 0;
	else
		_player.curretFrameY = 1;


	if (_animCount % _animSpeed == 0)
	{
		_animCount = 0;

		switch (_player.state)
		{
		case PLAYERSTATE_IDLE:
			break;

		case PLAYERSTATE_MOVING:
			_animSpeed = 3;
			_player.currentFrameX++;

			if (_player.currentFrameX >= _player.image->getMaxFrameX())  _player.currentFrameX = 0;

			break;

		case PLAYERSTATE_LOOKING_DOWN:
			break;

		case PLAYERSTATE_LOOKING_DOWN_MOVING:
			_animSpeed = 3;
			_player.currentFrameX++;

			if (_player.currentFrameX >= _player.image->getMaxFrameX()) _player.currentFrameX = 0;
			break;

		case PLAYERSTATE_JUMPING:
			_animSpeed = 15;

			_player.currentFrameX++;
			if (_player.currentFrameX >= 1) _player.currentFrameX = 1;
			break;
		case PLAYERSTATE_FALLING:
			_animSpeed = 5;

			if (_player.currentFrameX >= _player.image->getMaxFrameX()) _player.currentFrameX = _player.image->getMaxFrameX();
			else _player.currentFrameX++;
			break;

		case PLAYERSTATE_ATTACKING:
		case PLAYERSTATE_ATTACKING_JUMP:
			_animSpeed = 5;
			_animDelay++;

			if (_player.currentFrameX == 3) _player.currentFrameX++;
			if (_player.currentFrameX == 2) _player.currentFrameX++;
			if (_player.currentFrameX == 1) _player.currentFrameX++;
			if (_player.currentFrameX == 0 && _animDelay > 2) _player.currentFrameX++;
				



			break;
		case PLAYERSTATE_ATTACKING_3:
			break;
		case PLAYERSTATE_HIT:
			break;
		case PLAYERSTATE_HOLDING_WALL:
			break;
		case PLAYERSTATE_HOLDING_LADDERUP:
			_animSpeed = 5;

			if (_player.yspeed > 0) {
				if (_player.currentFrameX >= _player.image->getMaxFrameX()) _player.currentFrameX = 0;
				else _player.currentFrameX++;
			}
			break;
		case PLAYERSTATE_HOLDING_LADDERDOWN:
			break;
		case PLAYERSTATE_CHEKINGINVENTORY:
			break;
		case PLAYERSTATE_DEAD:
			_animSpeed = 3;

			if (_player.currentFrameX >= _player.image->getMaxFrameX()) _player.currentFrameX = _player.image->getMaxFrameX();
			else _player.currentFrameX++;
			break;

		default:
			break;
		}
	}


	_player.image->setX(_player.rc.left);
	_player.image->setY(_player.rc.top);
}

void Player::move()
{

	//state 별로 계산해야할 것
	switch (_player.state) {

	case PLAYERSTATE_JUMPING:
		// 중력
		_player.yspeed -= _player.gravity;
		// 중력 (최대 속도 제한 있음)
		if (_player.yspeed < -FALLPOWERMAX)
			_player.yspeed = -FALLPOWERMAX;

		// 좌우 키보드 뗄 시 수직 강하로 바뀜 (포물선처럼 보이게)
		if (_player.xspeed > 0)
			_player.xspeed -= 0.2;
		else if (_player.xspeed < 0)
			_player.xspeed += 0.2;

		//FALLING 상태로 변경
		if (_player.yspeed < 0) {
			_player.state = PLAYERSTATE_FALLING;
			setStateImg();
		}
	break;

	case PLAYERSTATE_FALLING:
	case PLAYERSTATE_ATTACKING_JUMP:
		// 중력
		_player.yspeed -= _player.gravity;
		// 중력 (최대 속도 제한 있음)
		if (_player.yspeed < -FALLPOWERMAX)
			_player.yspeed = -FALLPOWERMAX;

		// 좌우 키보드 뗄 시 수직 강하로 바뀜 (포물선처럼 보이게)
		if (_player.xspeed > 0)
			_player.xspeed -= 0.2;
		else if (_player.xspeed < 0)
			_player.xspeed += 0.2;
	break;


	case PLAYERSTATE_ATTACKING:	
		if (_player.xspeed > 0)
			_player.xspeed -= 0.2;
		else if (_player.xspeed < 0)
			_player.xspeed += 0.2;
	break;

	case PLAYERSTATE_HIT:
		_player.yspeed -= _player.gravity;
		_player.xspeed -= 0.1;

	break;

	case PLAYERSTATE_HOLDING_WALL:

		_player.xspeed = 0;

	break;

	case PLAYERSTATE_DEAD:
		// 중력
		_player.yspeed -= _player.gravity;
		// 중력 (최대 속도 제한 있음)
		if (_player.yspeed < -FALLPOWERMAX)
			_player.yspeed = -FALLPOWERMAX;
	break;
		
	}

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

	if (_player.state == PLAYERSTATE_DEAD)
		canMove = false;

	if (canMove)
	{
		//방향 설정
		if (_player.state != PLAYERSTATE_HOLDING_WALL &&
			_player.state != PLAYERSTATE_ATTACKING &&
			_player.state != PLAYERSTATE_ATTACKING_JUMP) {
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
				_player.lookingRight = true;
			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
				_player.lookingRight = false;
		}

		switch (_player.state) {
		case PLAYERSTATE_IDLE:

			_player.xspeed = 0;

			//올려다보는 모션
			if (KEYMANAGER->isStayKeyDown(VK_UP))
				_player.currentFrameX = 1;
			if (KEYMANAGER->isOnceKeyUp(VK_UP))
				_player.currentFrameX = 0;


			//이동
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed = RUNPOWERSTART;
				_player.state = PLAYERSTATE_MOVING;
				setStateImg();
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed = -RUNPOWERSTART;
				_player.state = PLAYERSTATE_MOVING;
				setStateImg();
			}



			if (KEYMANAGER->isStayKeyDown(VK_DOWN)) {
				_player.state = PLAYERSTATE_LOOKING_DOWN;
				setStateImg();
			}

			jump();
			holdLadder();
			canDown();
			attack();

			break;


		case PLAYERSTATE_MOVING:

			// 이동 중단
			if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();

			}

			//아래 보기

			if (KEYMANAGER->isStayKeyDown(VK_DOWN)) {
				_player.state = PLAYERSTATE_LOOKING_DOWN_MOVING;
				setStateImg();
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


			jump();
			holdLadder();
			canDown();
			attack();

			break;

		case PLAYERSTATE_LOOKING_DOWN:
			if (KEYMANAGER->isOnceKeyUp(VK_DOWN)) {
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}

			if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) {
				_player.xspeed = RUNPOWERSTART * 0.5;
				_player.state = PLAYERSTATE_LOOKING_DOWN_MOVING;
				setStateImg();
			}
			if (KEYMANAGER->isStayKeyDown(VK_LEFT)) {
				_player.xspeed = -RUNPOWERSTART * 0.5;
				_player.state = PLAYERSTATE_LOOKING_DOWN_MOVING;
				setStateImg();
			}

			attack();
			jump();
			canDown();

			break;

		case PLAYERSTATE_LOOKING_DOWN_MOVING:
			// 이동 중단
			if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_LOOKING_DOWN;
				setStateImg();
			}
			if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed = 0;
				_player.state = PLAYERSTATE_LOOKING_DOWN;
				setStateImg();
			}

			//일어서기
			if (KEYMANAGER->isOnceKeyUp(VK_DOWN)) {
				_player.state = PLAYERSTATE_MOVING;
				setStateImg();
			}

			// 가속
			if (KEYMANAGER->isStayKeyDown(VK_LEFT) && !_player.lookingRight) {
				_player.xspeed -= 1 * 0.5;
			}
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _player.lookingRight) {
				_player.xspeed += 1 * 0.5;
			}
			if (_player.xspeed > RUNPOWERMAX * 0.5)
				_player.xspeed = RUNPOWERMAX * 0.5;
			if (_player.xspeed < -RUNPOWERMAX * 0.5)
				_player.xspeed = -RUNPOWERMAX * 0.5;


			jump();
			holdLadder();
			canDown();
			attack();

			break;


		case PLAYERSTATE_JUMPING:

			// 꾹 누르면 더 높이 올라감
			if (KEYMANAGER->isStayKeyDown('Z') && _player.yspeed > 3) {
				_player.yspeed += 0.3;
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


			holdLadder();
			attackjump();

			break;

		case PLAYERSTATE_FALLING:


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


			holdLadder();
			attackjump();

			break;


		case PLAYERSTATE_ATTACKING:
			attackingNow();

			if (_player.currentFrameX >= _player.image->getMaxFrameX()) {
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}
			break;

		case PLAYERSTATE_ATTACKING_JUMP:
			attackingNow();

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

			if (_player.currentFrameX >= _player.image->getMaxFrameX()) {
				_player.state = PLAYERSTATE_FALLING;
				setStateImg();
			}
			break;

		case PLAYERSTATE_ATTACKING_3:

			break;
		case PLAYERSTATE_HIT:

			break;
		case PLAYERSTATE_HOLDING_WALL:

			//홀드 중에 점프
			//벽에 다시 붙지 않게 벽 반대방향으로 약간 이동한 뒤 점프파워를 가한다
			if (KEYMANAGER->isOnceKeyDown('Z')) {
				_player.yspeed = JUMPPOWERSTART * 0.8;
				if (_player.lookingRight) {
					_player.pointx -= 10;
					_player.xspeed = RUNPOWERSTART;
				}
				else {
					_player.pointx += 10;
					_player.xspeed = -RUNPOWERSTART;
				}
				_player.state = PLAYERSTATE_JUMPING;
				setStateImg();
			}


			attackjump();



			/*
			//홀드 중에 벽 반대방향으로 이동
			//벽 반대방향으로 떨어진다
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

			//홀드 중에 벽 아래방향으로 이동
			//벽에 다시 붙지 않게 벽 반대방향으로 약간 이동한 뒤 그대로 falling
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

			*/

			break;


		case PLAYERSTATE_HOLDING_LADDERUP:

			if ((midM.type == MAPTILE_LADDER || upM.type == MAPTILE_LADDER)
				&& KEYMANAGER->isStayKeyDown(VK_UP)) {
				_player.yspeed = LADDERUPSPEED;
			}
			else {
				_player.yspeed = 0;
			}


			if (KEYMANAGER->isOnceKeyDown(VK_DOWN)) {
				_player.pointy += 1;
				_player.state = PLAYERSTATE_HOLDING_LADDERDOWN;
				setStateImg();
			}

			jump();
			attackjump();

			break;


		case PLAYERSTATE_HOLDING_LADDERDOWN:


			if ((midM.type == MAPTILE_LADDER || botM.type == MAPTILE_LADDER)
				&& KEYMANAGER->isStayKeyDown(VK_DOWN))
			{
				_player.yspeed = LADDERDOWNSPEED;
			}
			else
				_player.yspeed = 0;

			if (KEYMANAGER->isOnceKeyDown(VK_UP)) {
				_player.pointy -= 1;
				_player.state = PLAYERSTATE_HOLDING_LADDERUP;
				setStateImg();
			}

			jump();
			attackjump();

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
	if (KEYMANAGER->isOnceKeyDown('Z')) {
		_player.yspeed = JUMPPOWERSTART;
		_player.state = PLAYERSTATE_JUMPING;
		_player.gravity = 0.4;
		setStateImg();
	}
}
void Player::attack()
{
	if (KEYMANAGER->isOnceKeyDown('X')) {
		float _offsetX, _offsetY;

		//수치는 임시 나중에 이미지 사이즈로 변경
		if (_equipWeapon.name == NAME_HAND)
		{
			_offsetY = 5;
			_offsetX = (_player.lookingRight) ? 5 : -5;
		}
		else {
			_offsetY = _equipWeapon.img0->getHeight();
			_offsetX = (_player.lookingRight) ? _equipWeapon.img0->getFrameWidth() : -_equipWeapon.img0->getFrameWidth();
		}

		if (_offsetX > 0)
			_equipWeaponRect.set(_player.pointx, _player.pointy, _player.pointx + _offsetX, _player.pointy + _offsetY);
		else
			_equipWeaponRect.set(_player.pointx + _offsetX, _player.pointy, _player.pointx, _player.pointy + _offsetY);

		_player.state = PLAYERSTATE_ATTACKING;
		setStateImg();
	}
}

void Player::attackjump()
{
	if (KEYMANAGER->isOnceKeyDown('X')) {
		float _offsetX, _offsetY;

		//수치는 임시
		if (_equipWeapon.name == NAME_HAND)
		{
			_offsetY = 5;
			_offsetX = (_player.lookingRight) ? 5 : -5;
		}
		else {
			_offsetY = _equipWeapon.img0->getHeight();
			_offsetX = (_player.lookingRight) ? _equipWeapon.img0->getFrameWidth() : -_equipWeapon.img0->getFrameWidth();
		}

		if (_offsetX > 0)
			_equipWeaponRect.set(_player.pointx, _player.pointy, _player.pointx + _offsetX, _player.pointy + _offsetY);
		else
			_equipWeaponRect.set(_player.pointx + _offsetX, _player.pointy, _player.pointx, _player.pointy + _offsetY);

		_player.state = PLAYERSTATE_ATTACKING_JUMP; 
		setStateImg();
	}
}

void Player::attackingNow() {

	float _offsetX = 0, _offsetY = 0;

	if (_player.currentFrameX == 0) { _offsetY = 0; _offsetX = (_player.lookingRight) ? 5 : -5;}
	if (_player.currentFrameX == 1) { _offsetY = 0; _offsetX = (_player.lookingRight) ? 25 : -25;}
	if (_player.currentFrameX == 2) { _offsetY = 2; _offsetX = (_player.lookingRight) ? 7 : -7;}
	if (_player.currentFrameX == 3) { _attackDelay = 1;}
	
	_equipWeaponRect.setCenterPos(_player.pointx + _offsetX, _player.pointy + _offsetY);
}

void Player::attackCollision() {

}

void Player::holdLadder()
{
	if (midM.type == MAPTILE_LADDER && KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		_player.pointx = midM.rc.left + (midM.rc.right - midM.rc.left) * 0.5;
		_player.pointy -= 1;
		_player.state = PLAYERSTATE_HOLDING_LADDERUP;
		_player.xspeed = 0;
		_player.yspeed = 0;
		setStateImg();
	}

}

void Player::canDown()
{
	if ((botM.type == MAPTILE_GROUND_CAN_GO_DOWN_1)
		&& KEYMANAGER->isStayKeyDown(VK_DOWN) && KEYMANAGER->isStayKeyDown('Z')) {
		_player.state = PLAYERSTATE_FALLING;
		_player.yspeed = 0;
		_player.pointy = botM.rc.top + (_player.rc.bottom - _player.rc.top) * 0.25;
		setStateImg();
	}


}

void Player::addStatusEffect(tagStatusEffect statuseffect)
{
	//상태이상 추가!
	for (int i = 0; i < 5; i++)
	{
		if (_player.statusEffect[i].type == statuseffect.type)
		{
			if (_player.statusEffect[i].leftTime < statuseffect.leftTime)
				_player.statusEffect[i] = statuseffect;
			break;
		}
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
			setStateImg();
		}

		//ATTAKING_JUMP라면 위로 보낸 뒤 계속 공격한다
		if (_player.state == PLAYERSTATE_ATTACKING_JUMP) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_ATTACKING;
		}

		//jump라면 위로 부딪힌 상황일테니 밑으로 튕겨낸다
		if (_player.state == PLAYERSTATE_JUMPING) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -3;
			_player.pointy = midM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			setStateImg();
		}

		//사다리 타고 올라가는 상태라면 더 못올라가게만 막는다
		if (_player.state == PLAYERSTATE_HOLDING_LADDERUP) {
			_player.pointy = midM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			_player.yspeed = 0;
		}


		if (_player.state == PLAYERSTATE_DEAD) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.yspeed = 0;
		}
		break;

		//내려갈 수 있는 발판의 경우 밑으로 튕겨낼 필요는 없다

	case MAPTILE_GROUND_CAN_GO_DOWN_1:
		//falling일 경우 아래로 부딪힌 상황이라면 위로 보내고 idle로 만든다
		//하지만 아래에서 접근하면서도 falling 상태일 수도 있다. 이건 플레이어 위치와 속도로 예외처리
		if (_player.state == PLAYERSTATE_FALLING
			&& _player.yspeed < -FALLPOWERMAX * 0.75) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5f);
			_player.currentFrameX = 8;
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_IDLE;
			setStateImg();
		}
		break;

	case MAPTILE_SPIKE_TRAP:
		if (_player.state == PLAYERSTATE_FALLING)
			getDamaged(999);
		break;
	}


	//botM
	//기본적인 바닥의 역할

	switch (botM.type)
	{
		//아무것도 없거나 트랩일 경우
	case MAPTILE_NULL:
	case MAPTILE_SPIKE_TRAP:

		//가만히 있거나 움직이는 상황이라면 떨어진다
		if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING
			|| _player.state == PLAYERSTATE_LOOKING_DOWN || _player.state == PLAYERSTATE_LOOKING_DOWN_MOVING) {
			_player.state = PLAYERSTATE_FALLING;
			setStateImg();
			_player.yspeed = -3;
		}

		//사다리를 타고 있다면 그 자리에서 멈춘다
		if (isCollision(_player.rc, botM.rc) && _player.state == PLAYERSTATE_HOLDING_LADDERDOWN) {
			_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
		}

		//공격중이라면 떨어지면서 계속 공격한다
		if (_player.state == PLAYERSTATE_ATTACKING) {
			_player.state = PLAYERSTATE_ATTACKING_JUMP;
		}
		break;


		//사다리 타일
	case MAPTILE_LADDER:
		//가만히 있거나 움직이는 상황이라면 떨어진다
		if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING
			|| _player.state == PLAYERSTATE_LOOKING_DOWN || _player.state == PLAYERSTATE_LOOKING_DOWN_MOVING) {
			_player.state = PLAYERSTATE_FALLING;
			setStateImg();
			_player.yspeed = -3;
		}

		//공격중이라면 떨어지면서 계속 공격한다
		if (_player.state == PLAYERSTATE_ATTACKING) {
			_player.state = PLAYERSTATE_ATTACKING_JUMP;
		}
		break;


		//벽&땅일경우
	case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1:
		//바닥에 착지 할 경우
		if (isCollision(_player.rc, botM.rc))
		{
			// 떨어지고 있거나 사다리 내려오는 중이라면 정상적으로 내려온다
			if (_player.state == PLAYERSTATE_FALLING || _player.state == PLAYERSTATE_HOLDING_LADDERDOWN) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
				_player.currentFrameX = 8;
				_player.xspeed = 0;
				_player.yspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
				setStateImg();
			}

			// 공격중이라면 착지후 그대로 계속 공격한다
			if (_player.state == PLAYERSTATE_ATTACKING_JUMP) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
				_player.xspeed = 0;
				_player.yspeed = 0;
				_player.state = PLAYERSTATE_ATTACKING;			
			}

			if (_player.state == PLAYERSTATE_DEAD) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			}
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

	//midR
	//오른쪽 벽, 못지나가게 함
	switch (midR.type) {

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (_player.rc.right > midR.rc.left) {
			_player.pointx = midR.rc.left - ((_player.rc.right - _player.rc.left) * 0.5);
			_player.xspeed = 0;


			if ((upR.type != MAPTILE_WALL && upR.type != MAPTILE_WALL2) &&
				(_player.rc.top > midR.rc.top - 10 && _player.rc.top < midR.rc.top) && !_invincible)
			{
				_player.pointy = midR.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5 + 2;
				_player.state = PLAYERSTATE_HOLDING_WALL;
				_player.xspeed = 0;
				_player.yspeed = 0;
				setStateImg();
			}
		}
		break;
	}


	//midL
	//왼쪽 벽, 못지나가게 함
	switch (midL.type) {

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (_player.rc.left < midL.rc.right) {
			_player.pointx = midL.rc.right + ((_player.rc.right - _player.rc.left) * 0.5);
			_player.xspeed = 0;
			
			if ((upL.type != MAPTILE_WALL && upL.type != MAPTILE_WALL2)
				&& (_player.rc.top > midL.rc.top - 10 && _player.rc.top < midL.rc.top) && !_invincible)
			{
				_player.pointy = midL.rc.top + (_player.rc.bottom - _player.rc.top) * 0.5 + 2;
				_player.state = PLAYERSTATE_HOLDING_WALL;
				_player.xspeed = 0;
				_player.yspeed = 0;
				setStateImg();
			}
		}
		break;
	}


	//upM
	switch (upM.type) {
	case MAPTILE_NULL: case MAPTILE_GROUND_CAN_GO_DOWN_1:
		if (isCollision(_player.rc, upM.rc) && _player.state == PLAYERSTATE_HOLDING_LADDERUP) {
			_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5;
		}
		break;

	case MAPTILE_WALL: case MAPTILE_WALL2:
		if (isCollision(_player.rc, upM.rc))
		{
			if (_player.state == PLAYERSTATE_JUMPING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = 0;
				_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
				setStateImg();
			}

			if (_player.state == PLAYERSTATE_HOLDING_LADDERUP) {
				_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5;
			}

			if (_player.state == PLAYERSTATE_ATTACKING_JUMP) {
				_player.yspeed = 0;
				_player.pointy = upM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
			}

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

void Player::enemyCollision() {
}


//공격 받았을 시 (데미지만)
void Player::getDamaged(int damage) {
	
	if (!_invincible) {
		_player.stat.hp -= damage;
		_ui->hitOutput(_player.pointx, _player.pointy, damage, LETTER_RED);
		
		_invincible = true;
		_invincibleTime = 1;
	}

	if (_player.stat.hp <= 0 && _player.state != PLAYERSTATE_DEAD) {
		_player.stat.hp = 0;
		_player.state = PLAYERSTATE_DEAD;
		setStateImg();
		_player.xspeed = 0;
		_player.yspeed -= 3;
	}

}
//공격 받았을 시 (데미지&넉백)

void Player::getDamaged(int damage, float angle, float knockbackpower) {
	if (!_invincible) {
		_player.stat.hp -= damage;

		_ui->hitOutput(_player.pointx, _player.pointy, damage, LETTER_RED);

		float xKnock = cosf(angle)*knockbackpower;
		float yKnock = (sinf(angle)*knockbackpower);

		yKnock = (yKnock > 0) ? yKnock : -yKnock;

		_player.xspeed = xKnock;
		_player.yspeed = yKnock;

		if (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP)
			_player.state == PLAYERSTATE_ATTACKING_JUMP;
		else
			_player.state = PLAYERSTATE_JUMPING;

		_invincible = true;
		_invincibleTime = 1;
	}


	if (_player.stat.hp <= 0 && _player.state != PLAYERSTATE_DEAD) {
		_player.stat.hp = 0;
		_player.state = PLAYERSTATE_DEAD;
		setStateImg();
		_player.xspeed = 0;
		_player.yspeed -= 3;
	}

}


//상태이상(준비중)
void Player::checkStatusEffect() {
	for (int i = 0; i < 5; i++) {
		_player.statusEffect[i].leftTime -= TIMEMANAGER->getElapsedTime();

		if (_player.statusEffect[i].leftTime < 0)
		{
			_player.statusEffect[i].type = STATUSEFFECT_NULL;
			break;
		}
	}
}

void Player::checkItemBox() {
	for (int i = 0; i < ITEMBOXMAX; i++) {
		tagObj box = _map->getitemBox(i);
		if (isCollision(_player.rc, box.rc))
		{
			_map->setItemBox(i, true);
		}
	}
	for (int i = 0; i < COINBOXMAX; i++) {
		tagObj box = _map->getCoinBox(i);
		if (isCollision(_player.rc, box.rc))
		{
			_map->setCoinBox(i, true);
		}
	}
}

void Player::checkDropItem() {
	_vItem = _ui->getvItem();
	
	for (int i = 0; i < _vItem.size(); i++) {
		tagItem item = _vItem[i];
		if (isCollision(_player.rc, item.rc))
		{
			_ui->setItemToBag(_vItem[i].name);
			_ui->deleteItemOnMap(i);
			
			break;
		}
	}
	
}

void Player::checkHitEnemy() {
	_attackDelay += TIMEMANAGER->getElapsedTime();

	if (_attackDelay > 1 && (_player.state == PLAYERSTATE_ATTACKING || _player.state == PLAYERSTATE_ATTACKING_JUMP))
	{
		_vEnemyRange = _em->getEnemyVector();

		for (int i = 0; i < _vEnemyRange.size() + 1; i++) {
			MYRECT enemyRect;
			RECT temp;
			if (i < _vEnemyRange.size()) {
				temp = _vEnemyRange[i]->getRect();
				enemyRect.set(temp.left, temp.top, temp.right, temp.bottom);

				if (isCollision(enemyRect, _equipWeaponRect) && _player.currentFrameX == 2) {
					_vEnemyRange[i]->getDamaged(10, getAngle(_player.pointx, _player.pointy, _vEnemyRange[i]->getPoint().x, _vEnemyRange[i]->getPoint().y), 3);
					_attackDelay = 0;
				}
			}




			else {
				temp = _em->getBoss()->getRect();
				enemyRect.set(temp.left, temp.top, temp.right, temp.bottom);
				if (isCollision(enemyRect, _equipWeaponRect) && _player.currentFrameX == 2) {
					_em->getBoss()->getDamaged(10, getAngle(_player.pointx, _player.pointy, _em->getBoss()->getPoint().x, _em->getBoss()->getPoint().y), 3);
					_attackDelay = 0;
				}
			}
		}
	}
}


void Player::checkInvincible() {
	if (_player.state != PLAYERSTATE_DEAD) {
		if (_invincible)
			_invincibleTime -= TIMEMANAGER->getElapsedTime();
		if (_invincibleTime < 0) {
			_invincible = false;
			_invincibleTime = 0;
		}
	}
}


void Player::firstSettingStat() {
	for (int i = 0; i < 5; i++)_player.statusEffect[i].type = STATUSEFFECT_NULL;

	_player.stat.hp = 100;
	_player.stat.str = 100;
	_player.stat.dex = 100;
	_player.stat.vit = 100;
	_player.stat.inl = 100;
	_player.stat.lck = 100;
	_player.stat.def = 100;
	_player.stat.fir = 100;
	_player.stat.ice = 100;
	_player.stat.lgt = 100;
	_player.stat.psn = 100;
	_player.stat.mel = 100;
	_player.stat.rng = 100;
	_player.stat.crit = 100;
	_player.stat.aspd = 100;
	_player.stat.spd = 100;
	_canCtrl = true;


	_player.jumppower = 0;
	_player.lookingLeft = true;
	_player.lookingRight = false;
	_player.xspeed = 0;
	_player.yspeed = 0;
	_player.money = 0;
	_player.gravity = 0.3;

	_player.state = PLAYERSTATE_FALLING;
	_player.currentFrameX = 0;
	_player.curretFrameY = 0;

}

void Player::mapcollision()
{

	if ((upL.type == MAPTILE_WALL || upL.type == MAPTILE_WALL2) && isCollisionReaction(upL.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((upM.type == MAPTILE_WALL || upM.type == MAPTILE_WALL2) && isCollisionReaction(upM.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((upR.type == MAPTILE_WALL || upR.type == MAPTILE_WALL2) && isCollisionReaction(upR.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((midL.type == MAPTILE_WALL || midL.type == MAPTILE_WALL2) && isCollisionReaction(midL.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((midM.type == MAPTILE_WALL || midM.type == MAPTILE_WALL2) && isCollisionReaction(midM.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;	
	}
	else if ((midR.type == MAPTILE_WALL || midR.type == MAPTILE_WALL2) && isCollisionReaction(midR.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((botL.type == MAPTILE_WALL || botL.type == MAPTILE_WALL2) && isCollisionReaction(botL.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((botM.type == MAPTILE_WALL || botM.type == MAPTILE_WALL2) && isCollisionReaction(botM.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
		
	}
	else if ((botR.type == MAPTILE_WALL || botR.type == MAPTILE_WALL2) && isCollisionReaction(botR.rc, _player.rc))
	{
		_player.pointy = _player.rc.top + (_player.rc.bottom - _player.rc.top) / 2;
		_player.pointx = _player.rc.left + (_player.rc.right - _player.rc.left) / 2;
	}

}
