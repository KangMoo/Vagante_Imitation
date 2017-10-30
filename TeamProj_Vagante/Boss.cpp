#include "stdafx.h"
#include "Boss.h"
#include "Player.h"
#include "UI.h"
#include "FireBall.h"

Boss::Boss()
{
}


Boss::~Boss()
{
}


HRESULT Boss::init()
{
	return S_OK;
}
HRESULT Boss::init(POINT point)
{
	//초기화해주세욤
	_image = IMAGEMANAGER->findImage("test");
	_currentFrameX = 0;
	_currentFrameY = 0;									//프레임
	_pointx = point.x;
	_pointy = point.y;										//좌표
	_xspeed = _yspeed = 0;												//x,y축 이동 속도
	_money = 10;															//몬스터 죽으면 나올 동전 갯수
	_isFindPlayer = false;													//플레이어를 발견한 상태인지
	_fireball = new FireBall;
	_fireball->init(100, 1000, "폭발");


	
	return S_OK;
}
void Boss::release()
{

}
void Boss::update()
{
	_fireball->setMapAddressLink(_map);
	_fireball->setPlayerAddressLink(_player);
	if (_isFindPlayer)
	{
		_isFindPlayer = true;
		move();
		jump();
		attack();

		_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());

		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > 150)
			_isFindPlayer = false;
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//프레임워크 수정에 의하여 _PlayerPoint를 _Player->getPoint()로 변경했습니다~~//
		///////////////////////////////////////////////////////////////////////////////////////

		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 50)
		{
			if (_pointx < _player->getPoint().x && _pointy < _player->getPoint().y)
			{
				//몬스터가 플레이어보다 왼쪽에 있고 위쪽에 있다
			}
			else if (_pointx < _player->getPoint().x && _pointy > _player->getPoint().y)
			{
				//몬스터가 플레이어보다 왼쪽에 있고 아래쪽에 있다
			}
			else if (_pointx > _player->getPoint().x && _pointy < _player->getPoint().y)
			{
				//몬스터가 플레이어보다 오른쪽에 있고 위쪽에 있다
			}
			else if (_pointx > _player->getPoint().x && _pointy > _player->getPoint().y)
			{
				//몬스터가 플레이어보다 오른쪽에 있고 아래쪽에 있다
			}
			else
			{

			}
		}
	}
	//test~
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 5);
	}
	_fireball->update();

	//~test
}
void Boss::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

//그릴 때	x좌표에 camera.x 만큼
//			y좌표에 camera.y 만큼 더해주기!!!!
void Boss::render(POINT camera)
{
	draw(camera);
}
void Boss::draw(POINT camera)
{
	_fireball->render(camera);
}
void Boss::move()

{

}
void Boss::jump()
{

}
void Boss::attack()
{

	//fireball 발사
	//_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 6);

}
void Boss::addStatusEffect(tagStatusEffect statuseffect)
{
	//상태이상 추가!
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL)
		{
			_statusEffect[i] = statuseffect;
			break;
		}
	}
}