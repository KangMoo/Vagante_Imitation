#include "stdafx.h"
#include "manEater.h"

manEater::manEater()
{
}
manEater::~manEater()
{
}

HRESULT manEater::init(POINT point, float minCog, float maxCog)
{
	_statistics.hp = 3;
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = 2;
	_isFindPlayer = false;
	_attack = false;
	_count = 0;

	_findRange = minCog;
	_findRangeMax = maxCog;

	return S_OK;
}

void manEater::release()
{

}

void manEater::update()
{

}

void manEater::render()
{

}

void manEater::render(POINT camera)
{

}

void manEater::draw(POINT camera)
{

}

void manEater::attack()
{
	if (_isFindPlayer == true && _statistics.hp >=0)
	{
		_count++;
		if (_count % 10 == 0)
		{
			//올라옴
			if (_attack == false)
			{
				IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() + 1); //올라가는 움직임
				_statistics.mel = RND->getFromIntTo(4, 14); //공격력 설정

				_state = ENEMYSTATE_ATTACKING; //상태 설정 

				_rc = RectMake(_pointx, _pointy, 32, 32);//공격 및 피격 렉트 설정
				_attackRect = _rc;						 //공격 및 피격 렉트 설정
			}

			//내려가는 중
			if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
			{
				_attack = true;
				IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX() + 1);//내려가는 움직임
			}

			//내려감
			if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
			{
				attRectClear();
				_rc = _attackRect; //렉트 초기화

				IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);		//프레임 초기화
				IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);	//프레임 초기화
			}
		}
	}

	if (_statistics.hp >= 0)
	{
		IMAGEMANAGER->findImage("maneater_Dead")->setFrameX(0);//죽었슴
	}
}
