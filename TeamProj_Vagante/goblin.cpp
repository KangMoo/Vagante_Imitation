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
	_statistics.hp = 16;
	_statistics.spd = 5;
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_right = false;

	_money = 2;
	_isFindPlayer = false;
	_attack = false;
	_count = 0;

	_findRange = minCog; // ���� ������ ����
	_findRangeMax = maxCog; //�þ� ����

	return S_OK;
}

void goblin::release()											
{

}

void goblin::update()											
{

}

void goblin::render()											
{

}

void goblin::render(POINT camera)								
{

}

void goblin::draw(POINT camera)									
{

}

void goblin::move()												
{
	if (_isFindPlayer == true && _statistics.hp >= 0) //�÷��̾� �߰� �� ü���� 0 �̻�
	{
		_state = ENEMYSTATE_MOVING;

		if (_pointx > _player->getPoint().x) //�÷��̾� ���� �÷��̾�� �����ʿ� ������ ������ �ٶ󺸰� ��
		{
			_right = false;
			_pointx -= _statistics.spd;
		}
		else //�÷��̾� ���� �÷��̾�� ���ʿ� ������ �������� �ٶ󺸰� ��
		{
			_right = true;
			_pointx += _statistics.spd;
		}
	}
	if (_isOnLand)  _state = ENEMYSTATE_IDLE;
}

void goblin::jump()	//�÷��̾� �߰��� ���¿��� �ϰ� �÷��̾ �ڽź��� ���� �־�� ��				
{

}

void goblin::attack()											
{
	if (_isFindPlayer && _statistics.hp >= 0)
	{
		if (_right == true && _player->getPoint().x <= _pointx + 32)
		{
			_attackRect = RectMake(_pointx + 32, _pointy + 8, 32, 16);
		}

		if (_right == false && _player->getPoint().x >= _pointx - 32)
		{
			_attackRect = RectMake(_pointx, _pointy + 8, 32, 16);
		}
	}
}

void goblin::falling()
{

}

void goblin::frameUpdate()
{

}