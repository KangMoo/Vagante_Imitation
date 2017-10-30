#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "UI.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}


HRESULT Enemy::init()
{
	return S_OK;
}
HRESULT Enemy::init(POINT point)
{
	//자식클래스에서 각자 초기화하기
	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
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
	
}
void Enemy::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top);
}

//그릴 때	x좌표에 camera.x 만큼
//			y좌표에 camera.y 만큼 더해주기!!!!
void Enemy::render(POINT camera)
{
	draw(camera);
}
void Enemy::draw(POINT camera)
{

}
void Enemy::move()

{

}
void Enemy::jump()			
{

}
void Enemy::attack()		
{

}
void Enemy::addStatusEffect(tagStatusEffect statuseffect)
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