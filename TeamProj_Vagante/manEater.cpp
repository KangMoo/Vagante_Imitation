#include "stdafx.h"
#include "manEater.h"
#include "Player.h"

manEater::manEater()
{
}
manEater::~manEater()
{
}

HRESULT manEater::init(POINT point, float minCog, float maxCog)
{
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = 2;
	_isFindPlayer = true;
	_attack = false;
	_count = 0;

	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);

	_findRange = minCog;
	_findRangeMax = maxCog;

	_attacki = new image;
	_attackAfteri = new image;
	_deadi = new image;
	
	_deadi->init("manEater_dead.bmp", 32, 32, true, RGB(255, 0, 255));
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

	_alpha = 255; 
	return S_OK;
}

void manEater::release()
{

}

void manEater::update()
{
	attack();//공격함수
	frameUpdate();//프레임을 움직여주는 함수

	if (_statistics.hp <= 0)
	{
		_image = _deadi;
	}
}

void manEater::render()
{

}

void manEater::render(POINT camera)
{
	draw(camera);
}

void manEater::draw(POINT camera)
{
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _rc.right + camera.x, _rc.bottom + camera.y);
	//_image->frameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0);
	//_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, 0);
	//Rectangle(getMemDC(), _rc.left + camera.x, _rc.top + camera.x, _rc.right + camera.x, _rc.bottom + camera.x);
	//
	//char *str;
	//
	//sprintf(str , "elapsedTime : %f", _currentFrameX);
	//TextOut(getMemDC(), _rc.left, _rc.top, str, strlen(str));

}

void manEater::attack()
{
	//if (_isFindPlayer == true && _statistics.hp >= 0)
	//{
		//_count++;
		//if (_count % 10 == 0)
		//{
		//	_count = 0;
		//	//올라옴
		//	if (_attack == false)
		//	{
		//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() + 1); //올라가는 움직임
		//		_statistics.mel = RND->getFromIntTo(4, 14); //공격력 설정
		//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);
		//		_state = ENEMYSTATE_ATTACKING; //상태 설정 
		//		;
	//
		//		//_rc = RectMake(_pointx, _pointy, 32, 32);//공격 및 피격 렉트 설정
		//		_attackRect = _rc;						 //공격 및 피격 렉트 설정
	//
		//		_image = IMAGEMANAGER->findImage("maneataer_Attack");
		//	}
	//
		//	//내려가는 중
		//	if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		//	{
		//		_attack = true;
		//		IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX() + 1);//내려가는 움직임
	//
		//		_image = IMAGEMANAGER->findImage("maneater_Attack_After");
	//
		//	}
	//
		//	//내려감
		//	if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		//	{
		//		attRectClear();
		//		_rc = _attackRect; //렉트 초기화
	//
		//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);		//프레임 초기화
		//		IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);	//프레임 초기화
		//	}
		//}
	//}
	
	//_frameTime += TIMEMANAGER->getElapsedTime();

	//이제 이미지가 돈다 만세!
	if (_currentFrameX > _image->getMaxFrameX() && _state == ENEMYSTATE_IDLE) // 공격하려고 올라온다
	{
		_rc = RectMake(_pointx, _pointy, 0, 0);//공격 및 피격렉트 설정 // 맞지않고 때려야하니 피격렉트 해제
		_attackRect = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);	 //공격 및 피격렉트 설정
		//_statistics.mel = RND->getFromIntTo(4, 15);			 //공격력
		_currentFrameX = 0;									 //프레임 초기화
		_image = _attackAfteri;								 //이미지 재설정
		_state = ENEMYSTATE_ATTACKING;						 //상태설정 
	}
	
	if (_currentFrameX >= _image->getMaxFrameX() && _state == ENEMYSTATE_ATTACKING) //공격하고 내려간다 
	{
		_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);//공격 및 피격렉트 설정
		_attackRect = RectMake(_pointx, _pointy, 0, 0);	 //공격 및 피격렉트 설정 // 줘맞아야 하니까 공격렉트 해제
		_statistics.mel = RND->getFromIntTo(4, 15);			 //공격력
		_currentFrameX = 0;									 //프레임 초기화
		_image = _image = _attacki;					         //이미지 재설정
		_state = ENEMYSTATE_IDLE;					      	 //상태설정 
	}

	RECT temp;

	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //플레이어를 공격했다.
	{
		_player->getDamaged(_statistics.mel);
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