#include "stdafx.h"
#include "maneater.h"

maneater::maneater()
{
}

maneater::~maneater()
{
}

/*
enum ENEMYSTATE {
	ENEMYSTATE_IDLE,
	ENEMYSTATE_MOVING,
	ENEMYSTATE_ATTACKING,
	ENEMYSTATE_HIT,
	ENEMYSTATE_JUMPING,
	ENEMYSTATE_FALLING,
	ENEMYSTATE_DEAD
};

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

//image* _image;														//이미지
//ENEMYSTATE _state;													//상태
//Player* _player;														//플레이어 정보
//UI* _ui;																//ui
//tagStatusEffect _statusEffect[5];										//상태이상
//tagStat _statistics;													//스탯
//int _currentFrameX, _currentFrameY;									//프레임
//RECT _rc;																//피격범위
//float _pointx, _pointy;												//좌표
//float _xspeed, _yspeed;												//x,y축 이동 속도
//int _money;															//몬스터 죽으면 나올 동전 갯수
//mapInfo _upL, _upM, _upR, _midL, _midM, _midR, _botL, _botM, _botR;	//현재좌표기준 9개 타일
//bool _isFindPlayer;													//플레이어를 발견한 상태인지

HRESULT maneater::init(POINT point)
{
	_statistics.hp = 3;
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = 2;
	_isFindPlayer = false;
	_count = 0;
	_delay = 0;
}
void maneater::release()
{

}
void maneater::update()
{
	attack();
}

void maneater::render(POINT camera)
{

}
void maneater::draw(POINT camera)
{

}						  
void maneater::attack()
{
	if (_isFindPlayer)
	{
		_state = ENEMYSTATE_ATTACKING;

		_count++;
		_delay++;
		//공격 다 했음
		if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX >=
			IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		{

			//집어넣기
			if (_delay % 10 == 0)
			{
				_delay = 0;
				IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX + 1);
			}

			//다 집어 넣었음
			if (IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX >=
				IMAGEMANAGER->findImage("maneater_Attack_After")->getMaxFrameX())
			{
				IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);
				IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);//초기화

				_rc = RectMakeCenter(_pointx, _pointy, 0, 0);//이제 맞지도 않고 공격도 못합니다.
			}

		}

		if (_count % 10 == 0)
		{
			_count = 0;
			IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX + 1);
			
			_rc = RectMakeCenter(_pointx, _pointy, 32, 32); //피격범위인데 공격범위도 피격범위랑 같음
			_statistics.mel = RND->getFromIntTo(4, 15);
			
			//데미지 전달하면 됨.

			//공격하는 구문입니다.
		}

	}
}
