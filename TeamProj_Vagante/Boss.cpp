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

	_minCog = 150;
	_maxCog = 1500;

	
	return S_OK;
}
void Boss::release()
{

}
void Boss::update()
{
	attRectClear();
	statusEffect();

	_fireball->setMapAddressLink(_map);
	_fireball->setPlayerAddressLink(_player);
	if (_isFindPlayer)
	{
		_isFindPlayer = true;
		move();
		jump();
		attack();


		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > _maxCog)
			_isFindPlayer = false;
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//프레임워크 수정에 의하여 _PlayerPoint를 _Player->getPoint()로 변경했습니다~~//
		///////////////////////////////////////////////////////////////////////////////////////

		//최초 인식상태의 몬스터와 플레이어의 거리가 기본 인식범위 사이일 때 연산 시작
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//몬스터와 플레이어가 같은 에어리어에 있다
				_isFindPlayer = true;
			}
			else
			{
				int count = 0;
				float x = 0;
				float y = 0;
				float dist = getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
				float angle = getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
				for (int i = 0; i < dist; i++)
				{
					float ox = (_pointx + i*cosf(angle)) / TILESIZE;
					float oy = (_pointy + i*-sinf(angle)) / TILESIZE;

					if (ox == x && oy == y) continue;

					x = ox;
					y = oy;

					if (static_cast<int>(_map->getMapInfo(y, x).type == 1))
						count++;
				}
				if (count >= 1) _isFindPlayer = false;
				else _isFindPlayer = true;
			}
		}
	}
	//test~
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		_fireball->fire(_pointx, _pointy, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 5);
	}
	_fireball->update();

	frameUpdate();
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
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
	if((_pointx > camera.x && _pointx < camera.x + WINSIZEX) &&
		(_pointy > camera.y && _pointy < camera.y + WINSIZEY))
		_image->frameRender(getMemDC(), _rc.left - camera.x, _rc.top - camera.y);
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

void Boss::statusEffect()
{
	for (int i = 0; i < 5; i++)
	{
		if (_statusEffect[i].type == NULL) continue;

		switch (_statusEffect[i].type)
		{
		case STATUSEFFECT_POISON:
			break;
		case STATUSEFFECT_FIRE:
			break;
		case STATUSEFFECT_STUN:
			break;
		case STATUSEFFECT_HEAL:
			break;
		}
	}
}

void Boss::frameUpdate()
{

}