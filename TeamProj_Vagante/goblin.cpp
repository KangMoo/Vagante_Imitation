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
	_state = ENEMYSTATE_IDLE; //���� �ʱ�ȭ
	_pointx = point.x;		  //����Ʈ �ʱ�ȭ
	_pointy = point.y;		  //����Ʈ �ʱ�ȭ
	_rc = RectMake(_pointx, _pointy, TILESIZE, TILESIZE); //�ǰݷ�Ʈ �ʱ�ȭ
	_money = RND->getFromIntTo(5, 1);

	_isFindPlayer = false;			 //�ʱ���� : �÷��̾ ã�� ����
	_attack = false;			     //�ʱ���� : �������� ����

	idleImg = new image;			 //�̹��� �ʱ�ȭ 
	moveImg = new image;			 //�̹��� �ʱ�ȭ 
	attackImg = new image;			 //�̹��� �ʱ�ȭ 
	attackImgLeft = new image;		 //�̹��� �ʱ�ȭ 
	attackImgRight = new image;		 //�̹��� �ʱ�ȭ 
	jumpImg = new image;			 //�̹��� �ʱ�ȭ 
	hitImg = new image;				 //�̹��� �ʱ�ȭ 
	deadImg = new image;			 //�̹��� �ʱ�ȭ 

	idleImg->init("Img\\enemy\\goblin_idle.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		//�⺻ �̹���
	moveImg->init("Img\\enemy\\goblin_moving.bmp", 256, 64, 8, 2, true, RGB(255, 0, 255));	//�����̴� �̹���
	jumpImg->init("Img\\enemy\\goblin_jump.bmp", 128, 64, 4, 2, true, RGB(255, 0, 255));	//�����ϴ� �̹���

	attackImg->init("Img\\enemy\\goblin_attack.bmp", 256, 64, 4, 2, true, RGB(255, 0, 255));//���� �̹���
	hitImg->init("Img\\enemy\\goblin_hit.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		//�´� �̹���
	deadImg->init("Img\\enemy\\goblin_dead.bmp", 32, 64, 1, 2, true, RGB(255, 0, 255));		//���� �̹���

	_findRange = minCog;		//�÷��̾� ã�� ����
	_findRangeMax = maxCog;		//�÷��̾� ã�� ����

	_statistics.hp = 16; //ü�� �ʱ�ȭ
	//_statistics.str = 1;
	_xspeed = _yspeed = 0; //���ǵ� �ʱ�ȭ

	_image = idleImg;	//�̹��� ���ۿ� ���� �̹��� �ʱ�ȭ
	_currentFrameX = 0;	//������ �ʱ�ȭ
	_currentFrameY = 0;	//������ �ʱ�ȭ
	_frameFPS = 10;		//������ �ѱ�°� �ʱ�ȭ
	_frameTime = 0;		//������ �ѱ�°� �ʱ�ȭ

	_dead = false;		//�������� �ʱ�ȭ	

	_deadAlpha = 0;		//������ ���ĸ� ������ �Ѵ�.

	_isOnLand = false;	//������ üũ
	_isLive = true;

	_jumptimer = TIMEMANAGER->getWorldTime();	//����Ÿ�̸� �ʱ�ȭ
	_gravity = 0;		//�߷� �ʱ�ȭ

	return S_OK;
}

void goblin::release()
{
}

void goblin::update()
{
	if (getDistance(_pointx, _pointy, 
		_player->getPoint().x, _player->getPoint().y) < 300) _isFindPlayer = true; //�÷��̾�� �Ÿ��� 300���ϸ� �÷��̾����� ����
	else _isFindPlayer = false; //300 �̻��̸� �߰� ����

	//�ӵ� ����
	if (_xspeed > 0) _xspeed -= 0.1;
	else if (_xspeed < 0) _xspeed += 0.1;
	if (_yspeed > 0) _yspeed -= 0.1;
	else if (_yspeed < 0) _yspeed += 0.1;

	//�ӵ� �Ѱ�ġ
	if (_xspeed > 2) _xspeed = 2;
	else if (_xspeed < -2) _xspeed = -2;

	mapCollisionCheck(); //�� üũ 
	attRectClear();	//���ݷ�Ʈ�� �� ���� �ʱ�ȭ�� ���ش�.
	frameUpdate(); //������ ������Ʈ�� ���ش�. �����ϸ� �� �׷���.
	
	//if (KEYMANAGER->isOnceKeyDown('N')) //��Ű�� ���� ������ �ְ� �״´�. 
	//{
	//	getDamaged(1);
	//	_image = hitImg;
	//}
	
	//if (PtInRect(&_findPlayerRect, _player->getPoint()) && _isLive == true); //�÷��̾ �߸� ã�Ұ� ü���� 0 �̻��϶�
	//{
	//	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); //�÷��̾ ������ �Ÿ��� 30��ŭ�̸� ���
	//	else move(); //�ƴϸ� �����δ�. // �÷��̾�� ���� �Ÿ��� 10�� �ɶ�����
	//	//jump();
	//}

	if (_isFindPlayer == true && _isLive == true) //�÷��̾ �߸� ã�Ұ� ü���� 0 �̻��϶�
	{
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); //�÷��̾ ������ �Ÿ��� 30��ŭ�̸� ���
		else move();
	}

	//if (_statistics.hp > 0) //�÷��̾ �߸� ã�Ұ� ü���� 0 �̻��϶�
	//{
	//	if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < 30) attack(); //�÷��̾ ������ �Ÿ��� 30��ŭ�̸� ���
	//	else move();
	//}

	if (_statistics.hp <= 0)//���üũ
	{
		_isLive = false;
		_image = deadImg; //���� �̹���
		_state = ENEMYSTATE_DEAD; //���¸� �����ŷ� �ٲ۴�
	}

	if (_state == ENEMYSTATE_DEAD)//�׾�����
	{
		_isLive = false;

		_deadAlpha += 5; //���İ��� ������Ų��
		if (_deadAlpha > 255)
		{
			_dead = true; // ���İ� 255 �̻��̸� �׾��ٰ� üũ�ϰ� ����.
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
	Rectangle(getMemDC(), _findPlayerRect.left, _findPlayerRect.top, _findPlayerRect.right, _findPlayerRect.bottom); //�� �׷������� �𸣰ڴٸ� ��Ʈ�� ������ �ʴ´�.
	_image->alphaFrameRender(getMemDC(), _rc.left + camera.x, _rc.top + camera.y, _currentFrameX, _currentFrameY, _deadAlpha); //ĳ���� ����� �׸���.
}

void goblin::move()
{
	_pointx += _xspeed; //���ǵ� ���ۿ� ������ �ξ��ٰ� �� ��ġ��ŭ ��ǥ���� ������
	_pointy += _yspeed;	//���ǵ� ���ۿ� ������ �ξ��ٰ� �� ��ġ��ŭ ��ǥ���� ������

	//�߷�ó��
	_yspeed += 0.3;

	_state = ENEMYSTATE_MOVING; //�����̴� ���·� ����

	_image = moveImg; //�̹����� �����̴°ŷ� �ٲ��ش�

	if (_pointx / TILESIZE > _player->getPoint().x / TILESIZE) //�÷��̾� ���� �÷��̾�� �����ʿ� ������ ������ �ٶ󺸰� ��
	{
		_right = false; //���������� üũ
		//_xspeed -= _statistics.spd; //�������ͽ� ���ǵ�� ���� �ʴ´�. �� �����ſ���
		_xspeed -= 0.5; //x���ǵ忡 ���� �ִ´�
	}
	else if ((_pointx / TILESIZE < _player->getPoint().x / TILESIZE)) //�÷��̾� ���� �÷��̾�� ���ʿ� ������ �������� �ٶ󺸸�
	{
		_right = true; //�������̴�
		//_xspeed += _statistics.spd;
		_xspeed += 0.5; //���ǵ忡 ���� �ִ´�

	} 
}

void goblin::jump()					
{
	_image = jumpImg; //�̹����� ������ �ٲ��ش�

	if (TIMEMANAGER->getWorldTime() - _jumptimer > 1) //������Ÿ��
	{
		_yspeed -= 8; //������
		_state = ENEMYSTATE_JUMPING;//�� ���´� �����̴�
		_jumptimer = TIMEMANAGER->getWorldTime();//����Ÿ�̸� �ʱ�ȸ
	}

	if (_state == ENEMYSTATE_JUMPING || _state == ENEMYSTATE_FALLING)
	{
		if (_yspeed > 0) _state = ENEMYSTATE_FALLING; //Yspeed�� 0���� ���ٸ� �������� ����
	
		if (_pointx > _player->getPoint().x) //�÷��̾� ���� �÷��̾�� �����ʿ� ������ ������ �ٶ󺸰� ��
		{
			_right = false; //�� �� ����Ʈ?
			_xspeed -= 2;
			_currentFrameY = 0;
		}
		else //�÷��̾� ���� �÷��̾�� ���ʿ� ������ �������� �ٶ󺸰� ��
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
	if (IntersectRect(&temp, &_player->getRect(), &_attackRect)) //�÷��̾ �����ߴ�.
	{
		_player->getDamaged(_statistics.mel, getAngle(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y), 3);
		//�÷��̾� �ݴ�������� ƨ�ܳ���
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