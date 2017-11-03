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
HRESULT Enemy::init(POINT point, float minCog, float maxCog)
{
	//자식클래스에서 각자 초기화하기
	_minCog = minCog;
	_maxCog = maxCog;

	_pointx = point.x;
	_pointy = point.y;

	_frameFPS = 0;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;

	_xspeed = _yspeed = _angle = _gravity = 0;

	_money = 0;

	_isFindPlayer = false;
	//_statusEffect[5]
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}

	memset(&_statistics, 0, sizeof(tagStat));

	_state = ENEMYSTATE_IDLE;
	
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, 1, 1);
	_lastPlayerPoint = _player->getPoint();
	_dead = false;
	_deadAlpha = 0;

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update() 
{
	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		getDamaged(10);
	}

	//공격용 렉트 정리해주는 함수, 만약 벌레같은 애들은 그냥 공격렉트가 똑같으니 그대로 처리
	//헤더파일에 있으니까 보고 수정 필요하면 재수정할것
	attRectClear();
	//상태이상효과 처리
	statusEffect();
	//낙하 처리
	falling();
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}

	if (_isFindPlayer && _state != ENEMYSTATE_DEAD)
	{
		//각자 움직이는 메커니즘이 다르므로 알아서 처리
		_isFindPlayer = true;
		move();
		jump();
		attack();

		//만약 둘 사이의 거리가 한계 인식범위 이상으로 벌어지면 쫓는걸 포기한다
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
				//몬스터와 플레이어가 같은 에어리어에 있다면 벽이든 뭐든 처리
				_isFindPlayer = true;
			}
			else
			{
				playerCog();
			}
		}
	}
	//프레임 업데이트와 rect는 거리에 관계없이 처리
	frameUpdate();
	rectResize();
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
	/*
	//Rectangle(getMemDC(), _pointx - _minCog / 2 + camera.x, _pointy - _minCog / 2 + camera.y, _pointx + _minCog / 2 + camera.x, _pointy + _minCog / 2 + camera.y);
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	if (_state != ENEMYSTATE_DEAD)
		_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y);
	else
		_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _deadAlpha);
	//EllipseMakeCenter(getMemDC(), _pointx + camera.x, _pointy + camera.y, 5, 5);

	
	char string[128];
	sprintf(string, "x : %d, y : %d", _rc.left / TILESIZE, _rc.top / TILESIZE);
	TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, string, strlen(string));
	if (_isFindPlayer)
	{
		sprintf(string, "ㅇㅇ");
		TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	else
	{
		sprintf(string, "ㄴㄴ");
		TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	*/
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


void Enemy::statusEffect()
{
	//스테이터스 이상시 효과를 주기위함
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

void Enemy::falling()
{
}

void Enemy::rectResize()
{
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void Enemy::playerCog()
{

	if (_lastPlayerPoint.x != _player->getPoint().x && _lastPlayerPoint.y != _player->getPoint().y)
	{
		_lastPlayerPoint = _player->getPoint();
		int count = 0;
		float x = 0;
		float y = 0;
		float dist = getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
		float angle = getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y);
		//적과 나 사이에 벽이 있는지 판별한다
		for (int i = 0; i < dist; i += TILESIZE)
		{
			//만약 지금 검사할 타일이 이미 검사할 타일과 같다면 다음으로 넘어간다
			float ox = (_pointx + i*cosf(angle)) / TILESIZE;
			float oy = (_pointy + i*-sinf(angle)) / TILESIZE;

			if (ox == x && oy == y) continue;

			//다르다면 x, y를 바꿔주고 그 타일을 검사한다
			x = ox;
			y = oy;

			if (static_cast<int>(_map->getMapInfo(y, x).type == MAPTILE_WALL))
			{
				count++;
				break;
			}
		}
		//만약 벽이 하나라도 검출되었다면 인식 못함으로 처리
		if (count >= 1) _isFindPlayer = false;
		else _isFindPlayer = true;
	}
}