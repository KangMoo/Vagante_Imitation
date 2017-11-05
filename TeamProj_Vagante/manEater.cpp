#include "stdafx.h"
#include "manEater.h"
#include "Player.h"

manEater::manEater()
{
}
manEater::~manEater()
{
}
				  //�����ʹ� mincog�� maxCog�� �����մϴ�.
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

	if (KEYMANAGER->isOnceKeyDown('M')) //��´°� Ȯ�ο�
	{
		getDamaged(1);
	}


	if (PtInRect(&_findRectRange,_player->getPoint())) //Ž��
	{
		_isFindPlayer = true;
	}

	if (_isFindPlayer == true)
	{
		attack();//�����Լ�
		frameUpdate();//�������� �������ִ� �Լ�
	}

	if (_statistics.hp <= 0)//���üũ
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
	Rectangle(getMemDC(), _findRectRange.left + camera.x, _findRectRange.top + camera.y, _findRectRange.right + camera.x, _findRectRange.bottom + camera.y); //�÷��̾� ��ġ Ž�� Ȯ�ο�
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
	//���� �̹����� ���� ����!
	if (_currentFrameX > _image->getMaxFrameX() && _state == ENEMYSTATE_IDLE) // �����Ϸ��� �ö�´�
	{
		_rc = RectMake(_pointx, _pointy, 0, 0);//���� �� �ǰݷ�Ʈ ���� // �����ʰ� �������ϴ� �ǰݷ�Ʈ ����
		_attackRect = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);	 //���� �� �ǰݷ�Ʈ ����
		_statistics.mel = RND->getFromIntTo(4, 15);			 //���ݷ�
		_currentFrameX = 0;									 //������ �ʱ�ȭ
		_image = _attackAfteri;								 //�̹��� �缳��
		_state = ENEMYSTATE_ATTACKING;						 //���¼��� 
	}
	
	if (_currentFrameX >= _image->getMaxFrameX() && _state == ENEMYSTATE_ATTACKING) //�����ϰ� �������� 
	{
		_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);//���� �� �ǰݷ�Ʈ ����
		_attackRect = RectMake(_pointx, _pointy, 0, 0);	 //���� �� �ǰݷ�Ʈ ���� // ��¾ƾ� �ϴϱ� ���ݷ�Ʈ ����
		//_statistics.mel = RND->getFromIntTo(4, 15);			 //���ݷ�
		_currentFrameX = 0;									 //������ �ʱ�ȭ
		_image = _image = _attacki;					         //�̹��� �缳��
		_state = ENEMYSTATE_IDLE;					      	 //���¼��� 

		_isFindPlayer = false;
	}

	RECT temp;

	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //�÷��̾ �����ߴ�.
	{
		_player->getDamaged(5, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), _statistics.mel);
		//�÷��̾� �ݴ�������� ƨ�ܳ���
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
	//�������� ������Ʈ �Ѵ�. �����ؼ� �ǵ��� ����

	_frameFPS = 10;
	_frameTime++;
	if (_frameFPS <= _frameTime)
	{
		_currentFrameX++;
		_frameTime = 0;
		//if (_statistics.hp <= 0) //������ ������°�
		//{
		//
		//}
	}
}