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
	_state = ENEMYSTATE_IDLE; //상태 초기화
	_pointx = point.x;		  //포인트 초기화
	_pointy = point.y;		  //포인트 초기화
	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE); //피격렉트 초기화
	_money = RND->getFromIntTo(5, 1);

	_isFindPlayer = false;			 //초기상태 : 플레이어를 찾지 못함
	_attack = false;			     //초기상태 : 공격하지 않음

	idleImg = new image;			 //이미지 초기화 
	moveImg = new image;			 //이미지 초기화 
	attackImg = new image;			 //이미지 초기화 
	attackImgLeft = new image;		 //이미지 초기화 
	attackImgRight = new image;		 //이미지 초기화 
	jumpImg = new image;			 //이미지 초기화 
	hitImg = new image;				 //이미지 초기화 
	deadImg = new image;			 //이미지 초기화 

	idleImg->init("Img\\enemy\\goblin_idle.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		//기본 이미지
	moveImg->init("Img\\enemy\\goblin_moving.bmp", 256, 64, 8, 2, true, RGB(255, 0, 255));	//움직이는 이미지
	jumpImg->init("Img\\enemy\\goblin_jump.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));	//점프하는 이미지

	attackImg->init("Img\\enemy\\goblin_attack.bmp", 256, 64, 4, 2, true, RGB(255, 0, 255));//공격 이미지
	hitImg->init("Img\\enemy\\goblin_hit.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		//맞는 이미지
	deadImg->init("Img\\enemy\\goblin_dead.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		//죽은 이미지

	_findRange = minCog;		//플레이어 찾는 범위
	_findRangeMax = maxCog;		//플레이어 찾는 범위

	_statistics.hp = 16; //체력 초기화
	//_statistics.str = 1;
	_xspeed = _yspeed = 0; //스피드 초기화

	_image = idleImg;	//이미지 버퍼에 담을 이미지 초기화
	_currentFrameX = 0;	//프레임 초기화
	_currentFrameY = 0;	//프레임 초기화
	_frameFPS = 10;		//프레임 넘기는거 초기화
	_frameTime = 0;		//프레임 넘기는거 초기화

	_dead = false;		//죽은상태 초기화	

	_deadAlpha = 0;		//죽으면 알파를 씌워야 한다.

	_isOnLand = false;	//땅인지 체크
	_isLive = true;

	_jumptimer = TIMEMANAGER->getWorldTime();	//점프타이머 초기화
	_gravity = 0;		//중력 초기화

	return S_OK;
}

void goblin::release()
{
}

void goblin::update()
{
	if (getDistance(_pointx, _pointy, 
		_player->getPoint().x, _player->getPoint().y) < 300) _isFindPlayer = true; //플레이어와 거리가 300이하면 플레이어한테 간다
	else _isFindPlayer = false; //300 이상이면 추격 포기

	//속도 감속
	if (_xspeed > 0) _xspeed -= 0.1;
	else if (_xspeed < 0) _xspeed += 0.1;
	if (_yspeed > 0) _yspeed -= 0.1;
	else if (_yspeed < 0) _yspeed += 0.1;

	//속도 한계치
	if (_xspeed > 2) _xspeed = 2;
	else if (_xspeed < -2) _xspeed = -2;

	mapCollisionCheck(); //맵 체크 
	attRectClear();	//공격렉트는 매 순간 초기화를 해준다.
	frameUpdate(); //프레임 업데이트를 해준다. 변경하면 좀 그렇다.
	
	//if (KEYMANAGER->isOnceKeyDown('N')) //엔키를 많이 누르면 애가 죽는다. 
	//{
	//	getDamaged(1);
	//	_image = hitImg;
	//}
	
	//if (PtInRect(&_findPlayerRect, _player->getPoint()) && _isLive == true); //플레이어가 야를 찾았고 체력이 0 이상일때
	//{
	//	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); //플레이어가 적보다 거리가 30만큼이면 찌른다
	//	else move(); //아니면 움직인다. // 플레이어와 적의 거리가 10이 될때까지
	//	//jump();
	//}

	if (_isFindPlayer == true && _isLive == true) //플레이어가 야를 찾았고 체력이 0 이상일때
	{
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); //플레이어가 적보다 거리가 30만큼이면 찌른다
		else move();
	}

	//if (_statistics.hp > 0) //플레이어가 야를 찾았고 체력이 0 이상일때
	//{
	//	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); //플레이어가 적보다 거리가 30만큼이면 찌른다
	//	else move();
	//}

	if (_statistics.hp <= 0)//사망체크
	{
		_isLive = false;
		_image = deadImg; //죽은 이미지
		_state = ENEMYSTATE_DEAD; //상태를 죽은거로 바꾼다
	}

	if (_state == ENEMYSTATE_DEAD)//죽었으면
	{
		_isLive = false;

		_deadAlpha += 5; //알파값을 증가시킨다
		if (_deadAlpha > 255)
		{
			_dead = true; // 알파가 255 이상이면 죽었다고 체크하고 뺀다.
		}
	}

	if (_state == ENEMYSTATE_HIT)
	{
		_image = hitImg;
		_state = ENEMYSTATE_IDLE;
	}
}

void goblin::render(POINT camera)
{
	draw(camera);
}

void goblin::draw(POINT camera)
{
	Rectangle(getMemDC(), _findPlayerRect.left, _findPlayerRect.top, _findPlayerRect.right, _findPlayerRect.bottom); //왜 그러지는지 모르겠다만 렉트가 보이지 않는다.
	_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, _currentFrameY, _deadAlpha); //캐릭터 모습을 그린다.
}

void goblin::move()
{
	_pointx += _xspeed; //스피드 버퍼에 저장해 두었다가 그 수치만큼 좌표값에 더해줌
	_pointy += _yspeed;	//스피드 버퍼에 저장해 두었다가 그 수치만큼 좌표값에 더해줌

	//중력처리
	_yspeed += 0.3;

	_state = ENEMYSTATE_MOVING; //움직이는 상태로 설정

	_image = moveImg; //이미지도 움직이는거로 바꿔준다

	if (_pointx / TILESIZE > _player->getPoint().x / TILESIZE) //플레이어 기준 플레이어보다 오른쪽에 있을때 왼쪽을 바라보게 함
	{
		_right = false; //오른쪽인지 체크
		//_xspeed -= _statistics.spd; //스테이터스 스피드는 쓰지 않는다. 왜 넣은거였지
		_xspeed -= 0.5; //x스피드에 값을 넣는다
	}
	else if ((_pointx / TILESIZE < _player->getPoint().x / TILESIZE)) //플레이어 기준 플레이어보다 왼쪽에 있을때 오른쪽을 바라보며
	{
		_right = true; //오른쪽이다
		//_xspeed += _statistics.spd;
		_xspeed += 0.5; //스피드에 값을 넣는다

	} 
}

void goblin::jump()					
{
	_image = jumpImg; //이미지를 점프로 바꿔준다

	if (TIMEMANAGER->getWorldTime() - _jumptimer > 1) //점프쿨타임
	{
		_yspeed -= 8; //점프력
		_state = ENEMYSTATE_JUMPING;//이 상태는 점핑이다
		_jumptimer = TIMEMANAGER->getWorldTime();//점프타이머 초기회
	}

	if (_state == ENEMYSTATE_JUMPING || _state == ENEMYSTATE_FALLING)
	{
		if (_yspeed > 0) _state = ENEMYSTATE_FALLING; //Yspeed가 0보다 적다면 떨어지는 상태
	
		if (_pointx > _player->getPoint().x) //플레이어 기준 플레이어보다 오른쪽에 있을때 왼쪽을 바라보게 함
		{
			_right = false; //유 어 롸이트?
			_xspeed -= 2;
			_currentFrameY = 0;
		}
		else //플레이어 기준 플레이어보다 왼쪽에 있을때 오른쪽을 바라보게 함
		{
			_right = true;
		//	_xspeed += _statistics.spd;
			_currentFrameY = 1;
		}
	}
	if (_isOnLand)  _state = ENEMYSTATE_IDLE;
}

void goblin::attack()
{

	_state = ENEMYSTATE_ATTACKING;

	_statistics.mel = RND->getFromIntTo(6, 7);

	if (_isFindPlayer && _statistics.hp >= 0)
	{
		_image = attackImg;

		if (_right == true && _player->getPoint().x <= _pointx + 32)
		{
			//_image = attackImgRight;
			_attackRect = RectMake(_pointx + 32, _pointy + 8, 32, 16);
		}
		
		if (_right == false && _player->getPoint().x >= _pointx - 32)
		{
			//_image = attackImgLeft;
			_attackRect = RectMake(_pointx, _pointy + 8, 32, 16);
		}
	}

	RECT temp;
	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //플레이어를 공격했다.
	{
		_player->getDamaged(_statistics.mel, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 3);
		//플레이어 반대방향으로 튕겨나기
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
	}

}

void goblin::falling()
{

}

void goblin::frameUpdate()
{
	_frameFPS = 10;
	_frameTime++;
	if (_frameFPS <= _frameTime)
	{
		_currentFrameX++;
		_frameTime = 0;
	}

	if (_currentFrameX > _image->getMaxFrameX())
	{
		_currentFrameX = 0;
	}

	if (_right == true && _state != ENEMYSTATE_ATTACKING) _currentFrameY =  0;
	else if (_right == false && _state != ENEMYSTATE_ATTACKING) _currentFrameY = 1;
}	

void goblin::playerCog()
{

}

void goblin::mapCollisionCheck()
{
	_upL = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE - 1);
	_upM = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE);
	_upR = _map->getMapInfo(int(_pointy) / TILESIZE - 1, int(_pointx) / TILESIZE + 1);
	_midL = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE - 1);
	_midM = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE);
	_midR = _map->getMapInfo(int(_pointy) / TILESIZE, int(_pointx) / TILESIZE + 1);
	_botL = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE - 1);
	_botM = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE);
	_botR = _map->getMapInfo(int(_pointy) / TILESIZE + 1, int(_pointx) / TILESIZE + 1);

	_rc = RectMakeCenter(_pointx, _pointy, TILESIZE, TILESIZE);
	RECT temp;
	if ((_midL.type == MAPTILE_WALL || _midL.type == MAPTILE_WALL2) && IntersectRect(&temp, &_midL.rc, &_rc))
	{
		_pointx += temp.right - temp.left;
		jump();
	}
	if ((_midR.type == MAPTILE_WALL || _midR.type == MAPTILE_WALL2) && IntersectRect(&temp, &_midR.rc, &_rc))
	{
		_pointx -= temp.right - temp.left;
		jump();
	}
	if ((_upM.type == MAPTILE_WALL || _upM.type == MAPTILE_WALL2) && IntersectRect(&temp, &_upM.rc, &_rc))
	{
		_pointy += temp.bottom - temp.top;
	}

	if (_player->getPoint().y >= _pointy)
	{
		if ((_botM.type == MAPTILE_WALL || _botM.type == MAPTILE_WALL2) && IntersectRect(&temp, &_botM.rc, &_rc))
		{
			_pointy -= temp.bottom - temp.top;
			_yspeed = 0;
			_isOnLand = false;
		}
	}
	else
	{
		if ((_botM.type == MAPTILE_WALL || _botM.type == MAPTILE_WALL2 || _botM.type == MAPTILE_GROUND_CAN_GO_DOWN_1) && IntersectRect(&temp, &_botM.rc, &_rc))
		{
			_pointy -= temp.bottom - temp.top;
			_yspeed = 0;
			_isOnLand = true;
		}
	}
	_rc = RectMakeCenter(_pointx, _pointy, TILESIZE, TILESIZE);

}