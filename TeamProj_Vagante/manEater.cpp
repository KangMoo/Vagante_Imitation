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
	attack();//�����Լ�
	frameUpdate();//�������� �������ִ� �Լ�

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
		//	//�ö��
		//	if (_attack == false)
		//	{
		//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() + 1); //�ö󰡴� ������
		//		_statistics.mel = RND->getFromIntTo(4, 14); //���ݷ� ����
		//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);
		//		_state = ENEMYSTATE_ATTACKING; //���� ���� 
		//		;
	//
		//		//_rc = RectMake(_pointx, _pointy, 32, 32);//���� �� �ǰ� ��Ʈ ����
		//		_attackRect = _rc;						 //���� �� �ǰ� ��Ʈ ����
	//
		//		_image = IMAGEMANAGER->findImage("maneataer_Attack");
		//	}
	//
		//	//�������� ��
		//	if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		//	{
		//		_attack = true;
		//		IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX() + 1);//�������� ������
	//
		//		_image = IMAGEMANAGER->findImage("maneater_Attack_After");
	//
		//	}
	//
		//	//������
		//	if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		//	{
		//		attRectClear();
		//		_rc = _attackRect; //��Ʈ �ʱ�ȭ
	//
		//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);		//������ �ʱ�ȭ
		//		IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);	//������ �ʱ�ȭ
		//	}
		//}
	//}
	
	//_frameTime += TIMEMANAGER->getElapsedTime();

	//���� �̹����� ���� ����!
	if (_currentFrameX > _image->getMaxFrameX() && _state == ENEMYSTATE_IDLE) // �����Ϸ��� �ö�´�
	{
		_rc = RectMake(_pointx, _pointy, 0, 0);//���� �� �ǰݷ�Ʈ ���� // �����ʰ� �������ϴ� �ǰݷ�Ʈ ����
		_attackRect = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);	 //���� �� �ǰݷ�Ʈ ����
		//_statistics.mel = RND->getFromIntTo(4, 15);			 //���ݷ�
		_currentFrameX = 0;									 //������ �ʱ�ȭ
		_image = _attackAfteri;								 //�̹��� �缳��
		_state = ENEMYSTATE_ATTACKING;						 //���¼��� 
	}
	
	if (_currentFrameX >= _image->getMaxFrameX() && _state == ENEMYSTATE_ATTACKING) //�����ϰ� �������� 
	{
		_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE);//���� �� �ǰݷ�Ʈ ����
		_attackRect = RectMake(_pointx, _pointy, 0, 0);	 //���� �� �ǰݷ�Ʈ ���� // ��¾ƾ� �ϴϱ� ���ݷ�Ʈ ����
		_statistics.mel = RND->getFromIntTo(4, 15);			 //���ݷ�
		_currentFrameX = 0;									 //������ �ʱ�ȭ
		_image = _image = _attacki;					         //�̹��� �缳��
		_state = ENEMYSTATE_IDLE;					      	 //���¼��� 
	}

	RECT temp;

	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //�÷��̾ �����ߴ�.
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