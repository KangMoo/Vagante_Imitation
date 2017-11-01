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
	//���� �����̴� �������ּ���~
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

	//������
	move();
	//Ÿ�ϰ� �б�
	setmaptileInfo();
	//���� �����ϸ� Ű �Է� �ޱ� (ui���� ������ �Ѿ ��� _canCtrl�� false)
	if (_canCtrl) keyintput();
}
void Player::render()
{

}
//�׸� ��	x��ǥ�� camera.x ��ŭ
//			y��ǥ�� camera.y ��ŭ �����ֱ�!!!!
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
	//�̵�
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

	//�̵�����
	bool canMove = true;
	//���������� üũ
	for (int i = 0; i < 5; i++)
	{
		if (_player.statusEffect[i].type == STATUSEFFECT_STUN) canMove = false;
	}
	//�ƴ϶�� �ൿ

	if (canMove)
	{

		//���� ����
		if (_player.state != PLAYERSTATE_HOLDING_WALL) {
			if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
				_player.lookingRight = true;
			if (KEYMANAGER->isStayKeyDown(VK_LEFT))
				_player.lookingRight = false;
		}
				
		switch (_player.state) {
		case PLAYERSTATE_IDLE:
			//�÷��ٺ��� ���
			if (KEYMANAGER->isStayKeyDown(VK_UP))
				_player.currentFrameX = 1;
			if (KEYMANAGER->isOnceKeyUp(VK_UP))
				_player.currentFrameX = 0;
			

			//�̵�
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

			//����
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

			// ����
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


			// ����
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


			// ����
			if (KEYMANAGER->isOnceKeyDown('X')) {
				_player.currentFrameX = 0;
				_player.yspeed = JUMPPOWERSTART;
				_player.state = PLAYERSTATE_JUMPING;
			}


			ladder();
			canDown();
			
			break;
		case PLAYERSTATE_JUMPING:
			// �߷�
			_player.yspeed -= 0.5;

			// �� ������ �� ���� �ö�
			if (KEYMANAGER->isStayKeyDown('X')) {
				_player.yspeed += 0.2;
			}

			// ���� �� �¿� �̵�
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

			// �¿� Ű���� �� �� ���� ���Ϸ� �ٲ� (������ó�� ���̰�)
			if (_player.xspeed > 0)
				_player.xspeed -= 0.2;
			else if (_player.xspeed < 0)
				_player.xspeed += 0.2;


			//FALLING ���·� ����
			if (_player.yspeed < 0)
				_player.state = PLAYERSTATE_FALLING;
			

			ladder();
			
			break;

		case PLAYERSTATE_FALLING:
			// �߷� (�ִ� �ӵ� ���� ����)
			if (_player.yspeed > -FALLPOWERMAX)
				_player.yspeed -= 0.5;
			else
				_player.yspeed = -FALLPOWERMAX;
			

			// ���� �� �¿� �̵�
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

			// �¿� Ű���� �� �� ���� ���Ϸ� �ٲ� (������ó�� ���̰�)
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
	//�߰�
}
void Player::attack()
{
	//�߰�
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
	//�����̻� �߰�!
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
	/*Ÿ������
	type

	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER, // ��ٸ�
	MAPTILE_GROUND_CAN_GO_DOWN_1, // ����
	MAPTILE_GROUND_CAN_GO_DOWN_2, // ����
	MAPTILE_TRAP_SPIKE,
	MAPTILE_TRAP_ARROW,
	MAPTILE_TRAP_ROCK
	*/
	//ĳ���� �������� 9���� Ÿ�� ���� �޾ƿ���
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

	//Ÿ�� Ȱ�� ����

	//�÷��̾� �Ʒ� Ÿ�� üũ

	//midM
	//1. (�ӵ� ���� ������) ���� ���� ��� �ѾƳ��� �Ѵ�

	switch (midM.type) {

	//���� ���, �� ���� ���� �ѾƳ��� �ȴ�
	case MAPTILE_WALL: case MAPTILE_WALL2: 

		//falling�̶�� �Ʒ��� �ε��� ��Ȳ���״� ���� ������ idle�� �����
		if (_player.state == PLAYERSTATE_FALLING) {
			_player.pointy = midM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
			_player.currentFrameX = 8;
			_player.xspeed = 0;
			_player.yspeed = 0;
			_player.state = PLAYERSTATE_IDLE;
		}

		//jump��� ���� �ε��� ��Ȳ���״� ������ ƨ�ܳ���
		if (_player.state == PLAYERSTATE_JUMPING) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -3;	
			_player.pointy = midM.rc.bottom + (_player.rc.bottom - _player.rc.top) * 0.5 + 1;
		}
	break;

	//������ �� �ִ� ������ ��� ������ ƨ�ܳ� �ʿ�� ����
	
	case MAPTILE_GROUND_CAN_GO_DOWN_1:
		//falling�� ��� �Ʒ��� �ε��� ��Ȳ�̶�� ���� ������ idle�� �����
		//������ �Ʒ����� �����ϸ鼭�� falling ������ ���� �ִ�. �̰� �÷��̾� ��ġ�� �ӵ��� ����ó��
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
		//�ƹ��͵� �������
	case MAPTILE_NULL:	case MAPTILE_LADDER:
		if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
			_player.state = PLAYERSTATE_FALLING;
			_player.yspeed = -3;
		}
	break;
		//��&���ϰ��
	case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1:
		//�ٴڿ� ���� �� ���
		if (isCollision(_player.rc, botM.rc))
		{
			if (_player.state == PLAYERSTATE_FALLING) {
				_player.pointy = botM.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
				_player.currentFrameX = 8;
				_player.xspeed = 0;
				_player.yspeed = 0;
				_player.state = PLAYERSTATE_IDLE;
			}
			//�߰�			
		}
	break;
		//��ٸ��ϰ��
	}


	/*
	//botR, botL
	//�밢�� �ؿ� ��ġ�� ��Ȳ ���

	//�����ŷ��� �ּ� ó����

	if (_player.state != PLAYERSTATE_IDLE && _player.state != PLAYERSTATE_MOVING) {
		switch (botR.type)
		{
			//�ƹ��͵� �������
		case MAPTILE_NULL:	case MAPTILE_LADDER:
			if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = -3;
			}
			break;
			//��&���ϰ��
		case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
			//�ٴڿ� ���� �� ���
			if (isCollision(_player.rc, botR.rc))
			{
				if (_player.state == PLAYERSTATE_FALLING) {
					_player.pointy = botR.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
					_player.currentFrameX = 8;
					_player.xspeed = 0;
					_player.yspeed = 0;
					_player.state = PLAYERSTATE_IDLE;
				}
				//�߰�			
			}
			break;
			//��ٸ��ϰ��
		}

		switch (botL.type)
		{
			//�ƹ��͵� �������
		case MAPTILE_NULL:	case MAPTILE_LADDER:
			if (_player.state == PLAYERSTATE_IDLE || _player.state == PLAYERSTATE_MOVING) {
				_player.state = PLAYERSTATE_FALLING;
				_player.yspeed = -3;
			}
			break;
			//��&���ϰ��
		case MAPTILE_WALL: case MAPTILE_WALL2:  case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
			//�ٴڿ� ���� �� ���
			if (isCollision(_player.rc, botL.rc))
			{
				if (_player.state == PLAYERSTATE_FALLING) {
					_player.pointy = botL.rc.top - ((_player.rc.bottom - _player.rc.top) * 0.5);
					_player.currentFrameX = 8;
					_player.xspeed = 0;
					_player.yspeed = 0;
					_player.state = PLAYERSTATE_IDLE;
				}
				//�߰�			
			}
			break;
			//��ٸ��ϰ��
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