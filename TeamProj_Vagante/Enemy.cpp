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
	//최소 인식범위, 최대 인식범위
	_minCog = minCog;
	_maxCog = maxCog;
	//현재 위치
	_pointx = point.x;
	_pointy = point.y;

	//프레임 변경용
	_frameFPS = 0;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;

	//날라갈때용
	_xspeed = _yspeed = _angle = _gravity = 0;
	//죽을때 뿌릴 돈
	_money = 0;
	//플레이어 찾았는지 여부
	_isFindPlayer = false;
	//_statusEffect[5]
	//상태이상 클리어
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}
	//스탯 클리어
	memset(&_statistics, 0, sizeof(tagStat));
	//현재 상태 설정(idle)
	_state = ENEMYSTATE_IDLE;
	//이미지 그릴떄 쓸 rect, 공격렉트, 마지막에 플레이어가 있던 좌표
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, 1, 1);
	_lastPlayerPoint = _player->getPoint();
	//죽었는지 여부 확인 및 투명처리용
	_dead = false;
	_deadAlpha = 0;

	return S_OK;
}
void Enemy::release()
{

}
void Enemy::update()
{
	//공격용 렉트 정리해주는 함수, 만약 벌레같은 애들은 그냥 공격렉트가 똑같으니 그대로 처리
	//헤더파일에 있으니까 보고 수정 필요하면 재수정할것
	attRectClear();
	//상태이상효과 처리
	statusEffect();
	//낙하 처리
	falling();
	//죽었으면 투명도 증가시키기
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}
	//죽지 않았을때만 적 확인등을 한다
	if (false)
	{
		//각자 움직이는 메커니즘이 다르므로 알아서 처리
		//일단 적 발견은 계속 true로
		_isFindPlayer = true;
		//이동
		move();
		//점프
		jump();

		//공격
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
				//인식범위 너무 길기도 하고 몹마다 달라서 virtual 함수로 빼놨습니다(제일 아래쪽)
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
	//그냥 적과 플레이어 사이에 선 하나 찍 그어놓고 선이 벽을 지나치는지 체크하는 정도
	//플레이어가 움직이지 않을때는 속도를 조금이라도 높이고자 바뀌었을때만 체크
	if (_lastPlayerPoint.x != _player->getPoint().x && _lastPlayerPoint.y != _player->getPoint().y)
	{
		//움직였다면 마지막 위치를 갱신해줍니다
		_lastPlayerPoint = _player->getPoint();
		//만약 벽이 있었으면 이걸 ++
		int count = 0;
		//몬스터와 플레이어 사이의 타일 x, y좌표를 위함
		float x = 0;
		float y = 0;
		//플레이어와 적 사이의 거리, 각도 체크용
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

			//해당 타일이 벽이라면 인식을 못했다고 처리해주고 연산 속도를 위해 바로 빠져나옵니다
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