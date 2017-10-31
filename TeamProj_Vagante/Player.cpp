#include "stdafx.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"
#include "UI.h"

//�װ�����

//��..

//�̾� �Ӵ���!!


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
	//�߰�
}
void Player::attack()
{
	//�߰�
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


	//Ÿ�� Ȱ�� ����

	//�÷��̾� �Ʒ� Ÿ�� üũ
	switch (botM.type)
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
			if (IntersectRect(&temp, &_player.rc, &botM.rc))
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