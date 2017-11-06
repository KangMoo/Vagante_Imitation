#include "stdafx.h"
#include "worm.h"
#include "Player.h"

worm::worm()
{
}


worm::~worm()
{
}
/*
struct tagStat {
	int hp;		//체력
	int str;	//힘
	int dex;	//민첩
	int vit;	//활력
	int inl;	//지능
	int lck;	//운
	int def;	//방어력
	int fir;	//불저항
	int ice;	//얼음저항
	int lgt;	//빛저항
	int psn;	//독저항
	int mel;	//근접공격데미지
	int rng;	//활공격데미지
	int crit;	//크리티컬확률
	int aspd;	//공속
	int spd;	//이속
};
*/

HRESULT worm::init(POINT point, float minCog, float maxCog)
{
	//_image = IMAGEMANAGER->findImage("wormMoveUp");
	//자식클래스에서 각자 초기화하기
	//인식,최대
	_minCog = minCog;
	_maxCog = maxCog;
	
	//최초 생성위치
	_pointx = point.x;
	_pointy = point.y;
	
	//프레임 변경 기준, 프레임, 현재 프레임
	_frameFPS = 10;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;
	//날라가는용
	_xspeed = _yspeed = _angle = _gravity = 0;
	//뿌릴 돈
	_money = RND->getFromIntTo(5, 1);

	//이미지 추가 처음할때 원래사이즈로 해서 *2로 표현해놓은거
	_moveLeft = new image;
	_moveLeft->init("Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveRight = new image;
	_moveRight->init("Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveUp = new image;
	_moveUp->init("Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	_moveDown = new image;
	_moveDown->init("Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	_hit = new image;
	_hit->init("Img\\enemy\\crawler_dead.bmp", 0, 0, 16 * 2, 24 * 2, 1, 2, true, RGB(255, 0, 255));

	//처음 
	_image = _moveUp;

	/*
	IMAGEMANAGER->addFrameImage("wormMoveUp", "Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveDown", "Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveLeft", "Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveRight", "Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	*/
	//처음엔 플레이어를 발견 못한다
	//마는 지렁이는 관계없이 움직여야하므로 바로 true로 바뀔것
	_isFindPlayer = false;
	//_statusEffect[5]
	//상태이상 적용x
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}
	//스탯 초기화
	memset(&_statistics, 0, sizeof(tagStat));
	//현재 상태 초기화
	_state = ENEMYSTATE_IDLE;
	//방향 설정
	int a = RND->getInt(2);
	//한번 설정한 방향은 죽을때까지 가지고 있는다
	if (a == 0) _isLeft = true;
	else _isLeft = false;
	//_isLeft = false;
	//프레임 끝까지 도달하면 다시 왔다갔다할거라 그거용
	_reverseFrame = false;
	//이동속도
	_moveSpeed = 0.5;
	//렉트, 공격렉트
	//지렁이는 기본적으로 똑같다
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	//지렁이 스탯 임의로 때려박기
	_statistics.hp = 10;
	_statistics.str = 2;
	_statistics.dex = 2;
	_statistics.vit = 2;
	_statistics.inl = 2;
	_statistics.lck = 2;
	_statistics.def = 2;
	_statistics.fir = 0;
	_statistics.ice = 0;
	_statistics.lgt = 0;
	_statistics.psn = 0;
	_statistics.mel = 0;
	_statistics.rng = 0;
	_statistics.crit = 0;
	_statistics.aspd = 0;
	_statistics.spd = 1;

	//지렁이 어딨는지 확인용
	//0 = 바닥, 1 = 왼쪽벽, 2 = 위쪽벽, 3 = 오른쪽벽
	_whereIsWorm = 0;


	//죽었는지 확인용
	_dead = false;
	_deadAlpha = 0;
	
	return S_OK;
}
void worm::update()
{

	//공격용 렉트 정리해주는 함수, 만약 벌레같은 애들은 그냥 공격렉트가 똑같으니 그대로 처리
	//헤더파일에 있으니까 보고 수정 필요하면 재수정할것
	attRectClear();
	//상태이상효과 처리
	statusEffect();
	//낙하 처리
	falling();

	//사망 후 투명도 변화
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		//완전히 투명해지면 매니저에서 지울 수 있도록 dead를 true
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
void worm::render(POINT camera) {
	if(_statistics.hp>0) draw(camera);
}
void worm::draw(POINT camera) {
	//Rectangle(getMemDC(), _pointx - _minCog / 2 + camera.x, _pointy - _minCog / 2 + camera.y, _pointx + _minCog / 2 + camera.x, _pointy + _minCog / 2 + camera.y);
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	if (_state != ENEMYSTATE_DEAD)
		_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y);
	else
		_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _deadAlpha);
	//EllipseMakeCenter(getMemDC(), _pointx + camera.x, _pointy + camera.y, 5, 5);



	/*
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
void worm::move()
{
	//일반적인 상태에서의 이동 처리, 타일판단등
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
	{
		//worm은 플레이어를 발견 하든 안하든 움직여야하므로 계속 moving으로 바꿔준다
		_state = ENEMYSTATE_MOVING;
		
		//바닥에 파묻혀있으면 꺼내줘야한다
		isDig();
		
		//_whereIsWorm은 벽이 어디 붙어있는지 판단하는 변수입니다
		//0은 바닥(이미지 wormMoveUp), 1은 왼쪽벽(이미지 wormMoveRight), 2는 윗쪽벽(이미지 wormMoveDown), 3은 오른쪽벽(이미지 wormMoveLeft)
		isThereWall();
		
		switch (_whereIsWorm)
		{
		case 0:
			if (_isLeft)
				_pointx -= _moveSpeed;
			else
				_pointx += _moveSpeed;
			break;
		case 1:
			if (_isLeft)
				_pointy -= _moveSpeed;
			else
				_pointy += _moveSpeed;
			break;
		case 2:
			if (_isLeft)
				_pointx += _moveSpeed;
			else
				_pointx -= _moveSpeed;
			break;
		case 3:
			if (_isLeft)
				_pointy += _moveSpeed;
			else
				_pointy -= _moveSpeed;
			break;
		}
	}
}
void worm::attack()
{
	//현재 상태가 움직이거나 일반적인 경우가 아니면 데미지를 주지 않는다
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
		_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	else
		_attackRect = RectMakeCenter(_pointx, _pointy, 0, 0);
	//플레이어랑 적 충돌 체크
	RECT temp;
	if (IntersectRect(&temp, &_player->getRect(), &_attackRect))
	{
		_player->getDamaged(_statistics.str, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 1);
		tagStatusEffect tse; 
		tse.type = STATUSEFFECT_POISON;
		tse.damage = 1;
		tse.leftTime = 5;
		_player->addStatusEffect(tse);
	}
}
void worm::frameUpdate()
{
	//왠지는 모르겠지만 _frameFPS 계속 정의 안해주면 이상함
	_frameFPS = 10;
	//사실 움직일때만 프레임 변동하고 맞을땐 상관없음
	if (_state == ENEMYSTATE_MOVING)
	{
		//0은 바닥에 붙어있을때, 1은 왼쪽벽에, 2는 천장에, 3은 오른쪽 벽에 붙어있음
		switch (_whereIsWorm)
		{
		case 0:
			//_image = IMAGEMANAGER->findImage("wormMoveUp");
			_image = _moveUp;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 1:
			//_image = IMAGEMANAGER->findImage("wormMoveRight");
			_image = _moveRight;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 2:
			//_image = IMAGEMANAGER->findImage("wormMoveDown");
			_image = _moveDown;
			if (_isLeft)
				_image->setFrameY(0);
			else _image->setFrameY(1);
			break;
		case 3:
			//_image = IMAGEMANAGER->findImage("wormMoveLeft");
			_image = _moveLeft;
			//얘만 왼쪽이랑 오른쪽이 반대임
			if (_isLeft)
				_image->setFrameY(1);
			else _image->setFrameY(0);
			break;
		}
		//if (_isLeft)
		//	_image->setFrameY(0);
		//else _image->setFrameY(1);
		_frameTime++;
		if (_frameTime >= _frameFPS)
		{
			_frameTime = 0;
			if (_reverseFrame)
				_currentFrameX--;
			else
				_currentFrameX++;
			if (_currentFrameX > _image->getMaxFrameX())
			{
				_reverseFrame = true;
				_currentFrameX = _image->getMaxFrameX();
			}
			else if (_currentFrameX < 0)
			{
				_reverseFrame = false;
				_currentFrameX = 1;
			}
			_image->setFrameX(_currentFrameX);
		}
	}
	else
	{
		//맞았을 때는 프레임 변동없이 그대로
		_image = _hit;
		_currentFrameX = 0;
		_frameFPS = 0;
		_image->setFrameX(0);
		if (_isLeft)
			_image->setFrameY(0);
		else
			_image->setFrameY(1);
	}
	/*
	ENEMYSTATE_IDLE,
		ENEMYSTATE_MOVING,
		ENEMYSTATE_ATTACKING,
		ENEMYSTATE_HIT,
		ENEMYSTATE_JUMPING,
		ENEMYSTATE_FALLING,
		ENEMYSTATE_DEAD
		*/
}

void worm::falling()	
{
	if (_state == ENEMYSTATE_HIT || _state == ENEMYSTATE_DEAD)
	{
		//먼저 지금 날라가는 방향에 타일의 타입이 벽인지 확인한다
		//이게 옆벽에 부딪히면 달라붙든가?
		//참고로 지렁이는 낙뎀 안받드라
		_gravity -= 0.1f;
		_yspeed += _gravity;

		if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed)/TILESIZE, (_pointx - cosf(_angle)*_xspeed)/TILESIZE).type == MAPTILE_WALL)
		{
			//위나 옆벽이면 튕겨야함
			//위인지부터 체크한다
			if (_yspeed < 0 && _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.y > _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.y)
			{
				//날라가는 방향의 타일 y값이 현재보다 크면 바닥에 부딪힌..거..겠지?
				_pointx = _pointx - cosf(_angle)*_xspeed;
				_pointy = _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx) / TILESIZE).rc.top - _moveUp->getFrameHeight() / 2;
				if(_state == ENEMYSTATE_HIT)	_state = ENEMYSTATE_IDLE;
				_whereIsWorm = 0;
				_xspeed = 0;
				_yspeed = 0;
				_angle = 0;
				_gravity = 0;
			}
			else if (_yspeed < 0 && _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).type == MAPTILE_SPIKE_TRAP)
			{
				//가시에 찔렸을 경우, 만약 죽은 경우만 아니면 데미지를 팍 준다
				if(_state != ENEMYSTATE_DEAD)
				{
					getDamaged(999);
					_whereIsWorm = 0;
					_xspeed = 0;
					_yspeed = 0;
					_angle = 0;
					_gravity = 0;
				}
			}
			else if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.y < _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.y)
			{
				//날라가는 방향 y값이 더 작으면 천장에 부딪혔으니 튕긴다->속도가 반대로
				_yspeed = -_yspeed;
			}
			else if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.x < _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.x ||
				_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.x > _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.x)
			{
				//날라가는 방향의 타일 x값이 현재와 다르면 결국 부딪힌거니 속도 반대로
				_xspeed = -_xspeed;
			}
			//else 
		}
		else
		{
			_pointx -= cosf(_angle)*_xspeed;
			_pointy += -sinf(_angle)*_yspeed;
		}
	}
}

void worm::rectResize()	
{
	//렉트사이즈를 재조정한다, 근데 얘는 뭐..
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void worm::isThereWall()
{
	//_whereIsWorm은 벽이 어디 붙어있는지 판단하는 변수입니다
	//0은 바닥(이미지 wormMoveUp), 1은 왼쪽벽(이미지 wormMoveRight), 2는 윗쪽벽(이미지 wormMoveDown), 3은 오른쪽벽(이미지 wormMoveLeft)
	//현재 진행방향 앞에 벽이 있는지, 앞에 벽이 없는데 그 아래도 벽이 없는지 판별합니다
	//현재 있는 타일 rect의 끝에 도달했는지 먼저 검사합시다
	switch (_whereIsWorm)
	{
	case 0:
		if (_isLeft)
		{
			//이번에 움직일 때 중심이 타일을 넘기는지 체크
			if ((_pointx - _moveSpeed)/TILESIZE < (_pointx)/TILESIZE)
			{
				//넘겼으면 변환을 시작
				//먼저 앞쪽에 벽이 있는지 계산
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//벽이 있다면 그 벽을 타고 올라가게 바꿔준다
					//모션은 1번(wormMoveRight)
					_whereIsWorm = 1;

					//x, y값을 보정한다
					//x는 앞쪽 벽(x-1) rect의 오른쪽에서 이미지 절반값만큼 더하고, y는 rect의 아래쪽에서 이속만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//만약 앞쪽에 벽이 없는데 그 아래에도 벽이 없을 경우 지금 자기 아래 타일을 타고 내려가야한다
					//x는 그대로, y는 + 1 해준 자기 앞 아래의 타일 검사, 또 자기 바로 앞 타일 검사
					//없으면 타고 내려간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 자기가 밟고있는 바닥(y+1)rect 왼쪽에서 이미지 절반값만큼 빼주고, y는 자기가 밟고있는 바닥rect 위에서 이속만큼 더해준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		else
		{
			//움직일 때 중심이 타일을 넘기는지 체크, _isLeft가 true일 때와는 다르다
			if ((_pointx + _moveSpeed) > (_pointx)/TILESIZE)
			{
				//역시 넘겼으면 변환을 시작
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으면 타고 올라간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 앞쪽 벽(x+1) rect의 왼쪽에서 이미지 절반값만큼 빼주고, y는 앞쪽 벽 rect의 아래쪽에서 이속만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy)/TILESIZE + 1, (_pointx + _moveSpeed)/TILESIZE).type != MAPTILE_WALL)
				{
					//역시 앞쪽에 벽 없는지 체크하고 아래도 체크해준다
					//둘다 없으면 1번(wormMoveRight)
					_whereIsWorm = 1;
					//값보정
					//x는 자기가 밟고있는 바닥(y+1)rect의 오른쪽에서 이미지 절반값만큼 더해주고, y는 자기가 밟고있는 바닥rect의 윗쪽에서 이속만큼 더한다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		break;
	case 1:
		
		if (_isLeft)
		{
			//중심을 넘기는지 체크, 이번엔 y값을 체크한다
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//넘겼으면 변환 시작
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으니 올라탄다
					//천장에 붙으므로 2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 위쪽 벽(y-1)rect의 왼쪽에서 이속만큼 더해주고, y는 위쪽벽 rect의 아래쪽에서 이미지 절반값만큼 더해준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight()/2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//위쪽에도 벽이 없고 그 아래에도 벽이 없으니 꺾인다
					//바닥에 붙으니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 밟고있는 바닥(x-1)rect의 오른쪽에서 이속만큼 빼주고, y는 밟고있는 바닥rect의 위쪽에서 이미지 절반값만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//지금 있는 타일보다 내려가는지 체크
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있다면 안착하니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 벽(y+1) rect의 왼쪽값에서 이속만큼 더해주고, y는 벽rect의 위쪽에서 이미지 절반값만큼 빼준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//아래쪽에도 벽이 없고 그 왼쪽에도 없으니 천장
					//2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 자기가 밟고있는 벽(x-1) rect의 오른쪽에서 이속만큼 빼주고, y는 밟고있는 벽 rect의 아래에서 이미지 절반값만큼 더해준다
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		
		break;
	case 2:
		//0번과 기본은 같다
		if (_isLeft)
		{
			//움직일 때 중심이 타일을 넘기는지 체크, _isLeft가 true일 때와는 다르다
			if ((_pointx + _moveSpeed) / TILESIZE > (_pointx) / TILESIZE)
			{
				//역시 넘겼으면 변환을 시작
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으면 타고 올라간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 앞쪽벽(x+1)의 왼쪽에서 이미지 절반값만큼 -, y는 위에서 이속만큼 +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth()/2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//역시 앞쪽에 벽 없는지 체크하고 아래도 체크해준다
					//둘다 없으면 1번(wormMoveRight)
					_whereIsWorm = 1;
					//값보정
					//x는 자기가 밟은벽(y - 1)의 오른쪽에서 이미지절반+, y는 자기가 밟은벽의 아래에서 이속만큼 빼줌
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom - _moveSpeed;
				}
			}
		}
		else
		{
			//이번에 움직일 때 중심이 타일을 넘기는지 체크
			if ((_pointx - _moveSpeed) / TILESIZE < (_pointx) / TILESIZE)
			{
				//넘겼으면 변환을 시작
				//먼저 앞쪽에 벽이 있는지 계산
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//벽이 있다면 그 벽을 타고 올라가게 바꿔준다
					//모션은 1번(wormMoveRight)
					_whereIsWorm = 1;

					//x, y값을 보정한다
					//x는 앞쪽벽(x-1)의 오른쪽에서 이미지 절반+, y는 앞쪽벽 위에서 이속만큼 +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//만약 앞쪽에 벽이 없는데 그 아래에도 벽이 없을 경우 지금 자기 아래 타일을 타고 내려가야한다
					//x는 그대로, y는 + 1 해준 자기 앞 아래의 타일 검사, 또 자기 바로 앞 타일 검사
					//없으면 타고 내려간다
					//3번(wormMoveLeft)
					_whereIsWorm = 3;
					//값보정
					//x는 밟은벽(y-1)왼쪽에서 이미지절반만큼-, y는 밟은벽 아래서 이속만큼-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + _moveSpeed;
				}
			}
		}
		break;
	case 3:
		if (_isLeft)
		{
			//지금 있는 타일보다 내려가는지 체크
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있다면 안착하니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 앞쪽벽(y+1)의 오른쪽에서 이속만큼-, y는 위에서 이미지절반만큼-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//아래쪽에도 벽이 없고 그 왼쪽에도 없으니 천장
					//2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 밟은벽(x + 1)왼쪽에서 이속만큼+, y는 아래에서 이미지 절반만큼+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//중심을 넘기는지 체크, 이번엔 y값을 체크한다
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//넘겼으면 변환 시작
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//앞쪽에 벽이 있으니 올라탄다
					//천장에 붙으므로 2번(wormMoveDown)
					_whereIsWorm = 2;
					//값보정
					//x는 앞쪽벽(y-1)의 오른쪽에서 이속만큼-, y는 아래에서 이미지 절반만큼+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//위쪽에도 벽이 없고 그 아래에도 벽이 없으니 꺾인다
					//바닥에 붙으니 0번(wormMoveUp)
					_whereIsWorm = 0;
					//값보정
					//x는 밟은벽(x+1)의 왼쪽에서 이속만큼+, y는 위에서 이미지 절반만큼 -
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		break;
	}
}

void worm::isDig()
{
	//상황에 따라 다르다
	switch (_whereIsWorm)
	{
	case 0:
		//바닥에 붙어있을때, 공중에 떠있는지와 바닥에 붙어있는지를 확인한다
		//먼저 바닥인지 확인(_moveUp)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//현재 있는 타일이 벽인 경우 바닥에 붙어있는 경우를 책정하므로 현재있는 타일 위로 올려준다
			//x는 변동없이 y만 올려주면 됨
			_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - _moveUp->getFrameHeight() / 2;
		}
		else if (_map->getMapInfo((_pointy + _moveUp->getFrameHeight()/2 + 1)/TILESIZE, (_pointx)/TILESIZE).type != MAPTILE_WALL)
		{
			//만약 현재 타일좌표와 자기 아래 타일좌표가 같다면 공중에 떠있으므로 위치를 내려준다
			_pointy += 5;
		}
		break;
	case 1:
		//바닥에 붙어있을때부터 체크, 이건 왼쪽벽에 붙어있을때다(_moveRight)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//x만 바꿔주면 됨
			_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.right + _moveRight->getFrameWidth() / 2;
		}
		else if (_map->getMapInfo((_pointy)/TILESIZE, (_pointx - _moveRight->getFrameWidth()/2 - 1)/TILESIZE).type != MAPTILE_WALL)
		{
			//역시 x만 바꿔주면 됨
			_pointx -= 5;
		}
		break;
	case 2:
		//천장(_moveDown)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//y만 변동, 바닥에 붙어있을때와는 반대다
			_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _moveDown->getFrameHeight() / 2;
		}
		else if (_map->getMapInfo((_pointy - _moveDown->getFrameHeight()/2 - 1)/TILESIZE, (_pointx)/TILESIZE).type != MAPTILE_WALL)
		{
			//y만 변동, 바닥에 붙어있을때와는 반대다
			_pointy -= 5;
		}
		break;
	case 3:
		//오른쪽 벽(_moveLeft)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//x만 변동, 왼벽관 반대
			_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.left - _moveLeft->getFrameWidth() / 2;
		}
		else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveLeft->getFrameWidth() / 2 + 1) / TILESIZE).type != MAPTILE_WALL)
		{
			//x만 변동, 왼벽관 반대
			_pointx += 5;
		}
		break;
	}
}