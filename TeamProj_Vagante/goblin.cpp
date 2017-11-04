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

	_findRange = minCog; // 공격 가능한 범위
	_findRangeMax = maxCog; //시야 범위

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
	if (_isFindPlayer == true && _statistics.hp >= 0) //플레이어 발견 및 체력이 0 이상
	{
		_state = ENEMYSTATE_MOVING;

		if (_pointx > _player->getPoint().x) //플레이어 기준 플레이어보다 오른쪽에 있을때 왼쪽을 바라보게 함
		{
			_right = false;
			_pointx -= _statistics.spd;
		}
		else //플레이어 기준 플레이어보다 왼쪽에 있을때 오른쪽을 바라보게 함
		{
			_right = true;
			_pointx += _statistics.spd;
		}
	}
	if (_isOnLand)  _state = ENEMYSTATE_IDLE;
}

void goblin::jump()	//플레이어 발견한 상태여야 하고 플레이어가 자신보다 위에 있어야 함				
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