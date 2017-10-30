#include "stdafx.h"
#include "maneater.h"

manEater::manEater()
{
}

manEater::~manEater()
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
int hp;		//ü��
int str;	//��
int dex;	//��ø
int vit;	//Ȱ��
int inl;	//����
int lck;	//��
int def;	//����
int fir;	//������
int ice;	//��������
int lgt;	//������
int psn;	//������
int mel;	//�������ݵ�����
int rng;	//Ȱ���ݵ�����
int crit;	//ũ��Ƽ��Ȯ��
int aspd;	//����
int spd;	//�̼�
};
*/

//image* _image;														//�̹���
//ENEMYSTATE _state;													//����
//Player* _player;														//�÷��̾� ����
//UI* _ui;																//ui
//tagStatusEffect _statusEffect[5];										//�����̻�
//tagStat _statistics;													//����
//int _currentFrameX, _currentFrameY;									//������
//RECT _rc;																//�ǰݹ���
//float _pointx, _pointy;												//��ǥ
//float _xspeed, _yspeed;												//x,y�� �̵� �ӵ�
//int _money;															//���� ������ ���� ���� ����
//mapInfo _upL, _upM, _upR, _midL, _midM, _midR, _botL, _botM, _botR;	//������ǥ���� 9�� Ÿ��
//bool _isFindPlayer;													//�÷��̾ �߰��� ��������

HRESULT manEater::init(POINT point)
{
	_statistics.hp = 3;
	_state = ENEMYSTATE_IDLE;
	_pointx = point.x;
	_pointy = point.y;

	_money = 2;
	_isFindPlayer = false;
	_attack = false;

	_count = 0;

	return S_OK;
}
void manEater::release()
{

}
void manEater::update()
{
	if (_isFindPlayer)
	{
		attack();
	}
}
void manEater::render()
{

}

void manEater::render(POINT camera)
{
	if (_state = ENEMYSTATE_ATTACKING)
	{
		IMAGEMANAGER->findImage("maneataer_Attack")->frameRender(getMemDC(), _pointx + camera.x, _pointy + camera.y);
	}

	if (_attack == true)
	{
		IMAGEMANAGER->findImage("maneater_Attack_After")->frameRender(getMemDC(), _pointx + camera.x, _pointy + camera.y);
	}
}

void manEater::draw(POINT camera)
{
}

void manEater::attack()
{
	//if (_isFindPlayer == true)
	//{
	//	_state = ENEMYSTATE_ATTACKING;
	//
	//	_count++;
	//	_delay++;
	//	//���� �� ����
	//	if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX >=
	//		IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
	//	{
	//
	//		//����ֱ�
	//		if (_delay % 10 == 0)
	//		{
	//			_delay = 0;
	//			IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX + 1);
	//		}
	//
	//		//�� ���� �־���
	//		if (IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX >=
	//			IMAGEMANAGER->findImage("maneater_Attack_After")->getMaxFrameX())
	//		{
	//			IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);
	//			IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);//�ʱ�ȭ
	//
	//			_rc = RectMakeCenter(_pointx, _pointy, 0, 0);//���� ������ �ʰ� ���ݵ� ���մϴ�.
	//		}
	//
	//	}
	//
	//	if (_count % 10 == 0)
	//	{
	//		_count = 0;
	//		IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX + 1);
	//
	//		_rc = RectMakeCenter(_pointx, _pointy, 32, 32); //�ǰݹ����ε� ���ݹ����� �ǰݹ����� ����
	//		_statistics.mel = RND->getFromIntTo(4, 15);
	//
	//		//������ �����ϸ� ��.
	//
	//		//�����ϴ� �����Դϴ�.
	//	}
	//
	//}

	_count++;
	if (_count % 10 == 0)
	{
		_count = 0;
		//���ݽõ�
		if (_attack == false) 
		{
			_state = ENEMYSTATE_ATTACKING;

			IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() + 1);

			_rc = RectMakeCenter(_pointx, _pointy, 32, 32); //�ǰݹ����ε� ���ݹ����� �ǰݹ����� ����
			_attackRange = _rc;
			_statistics.mel = RND->getFromIntTo(4, 15);
		}

		//������
		if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		{

			IMAGEMANAGER->findImage("maneataer_Attack")->setFrameX(0);//���� �̹��� �ʱ�ȭ

			_attack = true;
			IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(IMAGEMANAGER->findImage("maneater_Attack_After")->getFrameX() + 1);

			_rc = RectMakeCenter(_pointx, _pointy, 32, 32); //�ǰݹ����ε� ���ݹ����� �ǰݹ����� ����
			_attackRange = _rc;
		}

		//���ݳ�
		if (IMAGEMANAGER->findImage("maneataer_Attack")->getFrameX() >= IMAGEMANAGER->findImage("maneataer_Attack")->getMaxFrameX())
		{
			IMAGEMANAGER->findImage("maneater_Attack_After")->setFrameX(0);

			_state = ENEMYSTATE_IDLE;

			_attack = false;
			_rc = RectMakeCenter(_pointx, _pointy, 0, 0); //�ǰݹ����ε� ���ݹ����� �ǰݹ����� ����
			_attackRange = _rc;
		}
	}
}