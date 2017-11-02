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
}

void goblin::jump()	//플레이어 발견한 상태여야 하고 플레이어가 자신보다 위에 있어야 함				
{

}

void goblin::attack()											
{

	if (_isFindPlayer == true)
	{

	}
}

void goblin::falling()
{

}

bool goblin::attackAble()
{
	//고블린이 공격을 하기 위해 자신과 플레이어의 위치를 비교함, 하지만 단지 플레이어와 자신의 위치만 비교해서 공격을 함
	//단 플레이어가 오른쪽에 있을때랑 왼쪽에 있을땐 연산자가 달라져야 함
	//그것을 위해 만든 함수가 attackAble 함수
	int _attackRange;

	if (_right == true)
	{
		
		return false;
	}
	else
	{
		return false;
	}
}