#include "stdafx.h"
#include "manEater.h"
#include "Player.h"

manEater::manEater()
{
}
manEater::~manEater()
{
}
				  //맨이터는 mincog랑 maxCog랑 동일합니다.
HRESULT manEater::init(POINT point, float minCog, float maxCog)
{
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = RND(0,5);
	_isFindPlayer = false;
	_attack = false;
	_count = 0;

	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);

	_findRange = minCog = maxCog;

	_findRectRange = RectMakeCenter(_pointx + 16, _pointy + 16, _findRange, _findRange);

	_attacki = new image;
	_attackAfteri = new image;
	_deadi = new image;
	
	_deadi->init("manEater_dead.bmp", 32, 32,1,1, true, RGB(255, 0, 255));
	_attacki->init("manEater_attack.bmp", 64, 32, 2, 1, true, RGB(255, 0, 255));
	_attackAfteri->init("manEater_after_attack.bmp", 224, 32, 7, 1, true, RGB(255, 0, 255));

	_statistics.hp = 3;
	_statistics.str = 1;
	_statistics.dex = 0;
	_statistics.vit = 0;
	_statistics.inl = 0;
	_statistics.lck = 0;
	_statistics.def = 0;
	_statistics.fir = 0;
	_statistics.ice = 0;
	_statistics.lgt = 0;
	_statistics.psn = 0;
	_statistics.mel = 0;
	_statistics.rng = 0;
	_statistics.crit = 0;
	_statistics.aspd = 0;
	_statistics.spd = 0;

	//_image = IMAGEMANAGER->findImage("maneater_Attack_After");
	_image = _attacki;

	_currentFrameX = 0;
	_currentFrameY = 0;

	_frameFPS = 10;
	_frameTime = 0;
	
	_dead = false;

	_deadAlpha = 0;

	return S_OK;
}

void manEater::release()
{

}

void manEater::update()
{

	if (KEYMANAGER->isOnceKeyDown('M')) //줘맞는거 확인용
	{
		getDamaged(1);
	}


	if (PtInRect(&_findRectRange,_player->getPoint())) //탐색
	{
		_isFindPlayer = true;
	}

	if (_isFindPlayer == true)
	{
		attack();//공격함수
		frameUpdate();//프레임을 움직여주는 함수
	}

	if (_statistics.hp <= 0)//사망체크
	{
		_image = _deadi;
	}

	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		if (_deadAlpha > 255)
		{
			_dead = true;
		}
	}
}


void manEater::render(POINT camera)
{
	draw(camera);
}

void manEater::draw(POINT camera)
{
	Rectangle(getMemDC(), _findRectRange.left + camera.x, _findRectRange.top + camera.y, _findRectRange.right + camera.x, _findRectRange.bottom + camera.y); //플레이어 위치 탐색 확인용
	//_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0);
	_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0,_deadAlpha);

	//_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0);
	//Rectangle(getMemDC(), 50,50,100,100);
	//
	//char *str;
	//
	//sprintf(str , "elapsedTime : %f", _currentFrameX);
	//TextOut(getMemDC(), _rc.left, _rc.top, str, strlen(str));
}

void manEater::attack()
{
	//이제 이미지가 돈다 만세!
	if (_currentFrameX > _image->getMaxFrameX() && _state == ENEMYSTATE_IDLE) // 공격하려고 올라온다
	{
		_rc = RectMake(_pointx, _pointy, 0, 0);//공격 및 피격렉트 설정 // 맞지않고 때려야하니 피격렉트 해제
		_attackRect = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);	 //공격 및 피격렉트 설정
		_statistics.mel = RND->getFromIntTo(4, 15);			 //공격력
		_currentFrameX = 0;									 //프레임 초기화
		_image = _attackAfteri;								 //이미지 재설정
		_state = ENEMYSTATE_ATTACKING;						 //상태설정 
	}
	
	if (_currentFrameX >= _image->getMaxFrameX() && _state == ENEMYSTATE_ATTACKING) //공격하고 내려간다 
	{
		_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);//공격 및 피격렉트 설정
		_attackRect = RectMake(_pointx, _pointy, 0, 0);	 //공격 및 피격렉트 설정 // 줘맞아야 하니까 공격렉트 해제
		//_statistics.mel = RND->getFromIntTo(4, 15);			 //공격력
		_currentFrameX = 0;									 //프레임 초기화
		_image = _image = _attacki;					         //이미지 재설정
		_state = ENEMYSTATE_IDLE;					      	 //상태설정 

		_isFindPlayer = false;
	}

	RECT temp;

	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //플레이어를 공격했다.
	{
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), _statistics.mel);
		//플레이어 반대방향으로 튕겨나기
		_xspeed = cosf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;
		_yspeed = -sinf(getAngle(_player->getPoint().x, _player->getPoint().y, _pointx, _pointy)) * 2;

		tagStatusEffect tStatEff;
		tStatEff.type = STATUSEFFECT_NULL;
		tStatEff.damage = 0;
		tStatEff.leftTime = 0;
		_player->addStatusEffect(tStatEff);
	}
}

void manEater::frameUpdate()
{
	//프레임을 업데이트 한다. 웬만해선 건들지 말자

	_frameFPS = 10;
	_frameTime++;
	if (_frameFPS <= _frameTime)
	{
		_currentFrameX++;
		_frameTime = 0;
		//if (_statistics.hp <= 0) //죽으면 사라지는거
		//{
		//
		//}
	}
}