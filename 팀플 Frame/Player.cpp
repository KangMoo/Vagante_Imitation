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
	//���� �����ϸ� Ű �Է� �ޱ�
	if(_canCtrl) keyintput();
}
void Player::render()
{

}
//�׸� ��	x��ǥ�� (WINSIZEX/2 - camera.x) ��ŭ
//			y��ǥ�� (WINSIZEY/2 - camera.y) ��ŭ �����ֱ�!!!!
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
	//�̵�
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

	//�̵�����
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
	//�߰�
}
void Player::attack()
{
	//�߰�
}
void Player::collisionCheck()
{
	//ĳ���� �������� ��,��,��,���� Ÿ�ϰ� �о���̱�
	mapInfo uptile, lefttile, righttile, downtile;
	uptile = _map->getMapInfo(int(_player.pointy) % TILESIZE - 1, int(_player.pointx) % TILESIZE);
	downtile = _map->getMapInfo(int(_player.pointy) % TILESIZE + 1, int(_player.pointx) % TILESIZE);
	lefttile = _map->getMapInfo(int(_player.pointy) % TILESIZE, int(_player.pointx) % TILESIZE - 1);
	righttile = _map->getMapInfo(int(_player.pointy) % TILESIZE, int(_player.pointx) % TILESIZE + 1);


	//�÷��̾� �Ʒ� Ÿ�� üũ
	switch (downtile.type)
	{
		//�ƹ��͵� �������
	case MAPTILE_NULL:
		break;
		//��&���ϰ��
	case MAPTILE_WALL: case MAPTILE_WALL2:
		for (int i = 0; i < _player.yspeed; i++)
		{
			RECT temp;
			//�ٴڿ� ���� �� ���
			if (IntersectRect(&temp, &_player.rc, &downtile.rc))
			{
				//�߰�
				
			}
		}
		break;
		//��ٸ��ϰ��
	case MAPTILE_LADDER:
		break;
		//�����ϰ��
	case MAPTILE_GROUND_CAN_GO_DOWN_1: case MAPTILE_GROUND_CAN_GO_DOWN_2:
		break;
		//�����ϰ�� (�ʿ��� ó��)
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