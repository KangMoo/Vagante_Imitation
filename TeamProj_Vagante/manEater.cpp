#include "stdafx.h"
#include "manEater.h"

manEater::manEater()
{
}
manEater::~manEater()
{
}

HRESULT manEater::init(POINT point, float minCog, float maxCog)
{
	_statistics.hp = 3;
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = 2;
	_isFindPlayer = false;
	_attack = false;
	_count = 0;

	_findRange = minCog;
	_findRangeMax = maxCog;

	return S_OK;
}

void manEater::release()
{

}

void manEater::update()
{

}

void manEater::render()
{

}

void manEater::render(POINT camera)
{

}

void manEater::draw(POINT camera)
{

}

void manEater::attack()
{
	if (_isFindPlayer == true && _statistics.hp >=0)
	{
		_count++;
		if (_count % 10 == 0)
		{
			//�ö��
			if (_attack == false)
			{
				IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() + 1); //�ö󰡴� ������
				_statistics.mel = RND->getFromIntTo(4, 14); //���ݷ� ����

				_state = ENEMYSTATE_ATTACKING; //���� ���� 

				_rc = RectMake(_pointx, _pointy, 32, 32);//���� �� �ǰ� ��Ʈ ����
				_attackRect = _rc;						 //���� �� �ǰ� ��Ʈ ����
			}

			//�������� ��
			if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
			{
				_attack = true;
				IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX() + 1);//�������� ������
			}

			//������
			if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
			{
				attRectClear();
				_rc = _attackRect; //��Ʈ �ʱ�ȭ

				IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);		//������ �ʱ�ȭ
				IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);	//������ �ʱ�ȭ
			}
		}
	}

	if (_statistics.hp >= 0)
	{
		IMAGEMANAGER->findImage("maneater_Dead")->setFrameX(0);//�׾���
	}
}
