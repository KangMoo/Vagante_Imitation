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

HRESULT worm::init(POINT point, float minCog, float maxCog)
{
	//_image = IMAGEMANAGER->findImage("wormMoveUp");
	//�ڽ�Ŭ�������� ���� �ʱ�ȭ�ϱ�
	//�ν�,�ִ�
	_minCog = minCog;
	_maxCog = maxCog;
	
	//���� ������ġ
	_pointx = point.x;
	_pointy = point.y;
	
	//������ ���� ����, ������, ���� ������
	_frameFPS = 10;
	_frameTime = 0;
	_currentFrameX = _currentFrameY = 0;
	//���󰡴¿�
	_xspeed = _yspeed = _angle = _gravity = 0;
	//�Ѹ� ��
	_money = RND->getFromIntTo(5, 1);

	//�̹��� �߰� ó���Ҷ� ����������� �ؼ� *2�� ǥ���س�����
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

	//ó�� 
	_image = _moveUp;

	/*
	IMAGEMANAGER->addFrameImage("wormMoveUp", "Img\\enemy\\crawler_move_up.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveDown", "Img\\enemy\\crawler_move_down.bmp", 0, 0, 64 * 2, 20 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveLeft", "Img\\enemy\\crawler_move_left.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("wormMoveRight", "Img\\enemy\\crawler_move_right.bmp", 0, 0, 40 * 2, 32 * 2, 4, 2, true, RGB(255, 0, 255));
	*/
	//ó���� �÷��̾ �߰� ���Ѵ�
	//���� �����̴� ������� ���������ϹǷ� �ٷ� true�� �ٲ��
	_isFindPlayer = false;
	//_statusEffect[5]
	//�����̻� ����x
	for (int i = 0; i < 5; i++)
	{
		_statusEffect[i].damage = 0;
		_statusEffect[i].leftTime = 0;
		_statusEffect[i].type = STATUSEFFECT_NULL;
	}
	//���� �ʱ�ȭ
	memset(&_statistics, 0, sizeof(tagStat));
	//���� ���� �ʱ�ȭ
	_state = ENEMYSTATE_IDLE;
	//���� ����
	int a = RND->getInt(2);
	//�ѹ� ������ ������ ���������� ������ �ִ´�
	if (a == 0) _isLeft = true;
	else _isLeft = false;
	//_isLeft = false;
	//������ ������ �����ϸ� �ٽ� �Դٰ����ҰŶ� �װſ�
	_reverseFrame = false;
	//�̵��ӵ�
	_moveSpeed = 0.5;
	//��Ʈ, ���ݷ�Ʈ
	//�����̴� �⺻������ �Ȱ���
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	//������ ���� ���Ƿ� �����ڱ�
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

	//������ ������� Ȯ�ο�
	//0 = �ٴ�, 1 = ���ʺ�, 2 = ���ʺ�, 3 = �����ʺ�
	_whereIsWorm = 0;


	//�׾����� Ȯ�ο�
	_dead = false;
	_deadAlpha = 0;
	
	return S_OK;
}
void worm::update()
{

	//���ݿ� ��Ʈ �������ִ� �Լ�, ���� �������� �ֵ��� �׳� ���ݷ�Ʈ�� �Ȱ����� �״�� ó��
	//������Ͽ� �����ϱ� ���� ���� �ʿ��ϸ� ������Ұ�
	attRectClear();
	//�����̻�ȿ�� ó��
	statusEffect();
	//���� ó��
	falling();

	//��� �� ���� ��ȭ
	if (_state == ENEMYSTATE_DEAD)
	{
		_deadAlpha += 5;
		//������ ���������� �Ŵ������� ���� �� �ֵ��� dead�� true
		if (_deadAlpha >= 255)
		{
			_dead = true;
		}
	}

	
	if (_isFindPlayer && _state != ENEMYSTATE_DEAD)
	{
		//���� �����̴� ��Ŀ������ �ٸ��Ƿ� �˾Ƽ� ó��
		_isFindPlayer = true;
		move();
		jump();
		attack();

		//���� �� ������ �Ÿ��� �Ѱ� �νĹ��� �̻����� �������� �Ѵ°� �����Ѵ�
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) > _maxCog)
			_isFindPlayer = false;

	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////
		//�����ӿ�ũ ������ ���Ͽ� _PlayerPoint�� _Player->getPoint()�� �����߽��ϴ�~~//
		///////////////////////////////////////////////////////////////////////////////////////

		//���� �νĻ����� ���Ϳ� �÷��̾��� �Ÿ��� �⺻ �νĹ��� ������ �� ���� ����
		if (getDistance(_pointx, _pointy, _player->getPoint().x, _player->getPoint().y) < _minCog)
		{
			if (static_cast<int>(_pointx / TILESIZE) == static_cast<int>(_player->getPoint().x / TILESIZE) &&
				static_cast<int>(_pointy / TILESIZE) == static_cast<int>(_player->getPoint().y / TILESIZE))
			{
				//���Ϳ� �÷��̾ ���� ���� �ִٸ� ���̵� ���� ó��
				_isFindPlayer = true;
			}
			else
			{
				playerCog();
			}
		}
	}
	//������ ������Ʈ�� rect�� �Ÿ��� ������� ó��
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
	sprintf(string, "����");
	TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	else
	{
	sprintf(string, "����");
	TextOut(getMemDC(), 300, 300, string, strlen(string));
	}
	*/
}
void worm::move()
{
	//�Ϲ����� ���¿����� �̵� ó��, Ÿ���Ǵܵ�
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
	{
		//worm�� �÷��̾ �߰� �ϵ� ���ϵ� ���������ϹǷ� ��� moving���� �ٲ��ش�
		_state = ENEMYSTATE_MOVING;
		
		//�ٴڿ� �Ĺ��������� ��������Ѵ�
		isDig();
		
		//_whereIsWorm�� ���� ��� �پ��ִ��� �Ǵ��ϴ� �����Դϴ�
		//0�� �ٴ�(�̹��� wormMoveUp), 1�� ���ʺ�(�̹��� wormMoveRight), 2�� ���ʺ�(�̹��� wormMoveDown), 3�� �����ʺ�(�̹��� wormMoveLeft)
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
	//���� ���°� �����̰ų� �Ϲ����� ��찡 �ƴϸ� �������� ���� �ʴ´�
	if (_state == ENEMYSTATE_IDLE || _state == ENEMYSTATE_MOVING)
		_attackRect = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
	else
		_attackRect = RectMakeCenter(_pointx, _pointy, 0, 0);
	//�÷��̾�� �� �浹 üũ
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
	//������ �𸣰����� _frameFPS ��� ���� �����ָ� �̻���
	_frameFPS = 10;
	//��� �����϶��� ������ �����ϰ� ������ �������
	if (_state == ENEMYSTATE_MOVING)
	{
		//0�� �ٴڿ� �پ�������, 1�� ���ʺ���, 2�� õ�忡, 3�� ������ ���� �پ�����
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
			//�길 �����̶� �������� �ݴ���
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
		//�¾��� ���� ������ �������� �״��
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
		//���� ���� ���󰡴� ���⿡ Ÿ���� Ÿ���� ������ Ȯ���Ѵ�
		//�̰� ������ �ε����� �޶�ٵ簡?
		//����� �����̴� ���� �ȹ޵��
		_gravity -= 0.1f;
		_yspeed += _gravity;

		if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed)/TILESIZE, (_pointx - cosf(_angle)*_xspeed)/TILESIZE).type == MAPTILE_WALL)
		{
			//���� �����̸� ƨ�ܾ���
			//���������� üũ�Ѵ�
			if (_yspeed < 0 && _map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.y > _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.y)
			{
				//���󰡴� ������ Ÿ�� y���� ���纸�� ũ�� �ٴڿ� �ε���..��..����?
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
				//���ÿ� ����� ���, ���� ���� ��츸 �ƴϸ� �������� �� �ش�
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
				//���󰡴� ���� y���� �� ������ õ�忡 �ε������� ƨ���->�ӵ��� �ݴ��
				_yspeed = -_yspeed;
			}
			else if (_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.x < _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.x ||
				_map->getMapInfo((_pointy + -sinf(_angle)*_yspeed) / TILESIZE, (_pointx - cosf(_angle)*_xspeed) / TILESIZE).point.x > _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).point.x)
			{
				//���󰡴� ������ Ÿ�� x���� ����� �ٸ��� �ᱹ �ε����Ŵ� �ӵ� �ݴ��
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
	//��Ʈ����� �������Ѵ�, �ٵ� ��� ��..
	_rc = RectMakeCenter(_pointx, _pointy, _image->getFrameWidth(), _image->getFrameHeight());
}

void worm::isThereWall()
{
	//_whereIsWorm�� ���� ��� �پ��ִ��� �Ǵ��ϴ� �����Դϴ�
	//0�� �ٴ�(�̹��� wormMoveUp), 1�� ���ʺ�(�̹��� wormMoveRight), 2�� ���ʺ�(�̹��� wormMoveDown), 3�� �����ʺ�(�̹��� wormMoveLeft)
	//���� ������� �տ� ���� �ִ���, �տ� ���� ���µ� �� �Ʒ��� ���� ������ �Ǻ��մϴ�
	//���� �ִ� Ÿ�� rect�� ���� �����ߴ��� ���� �˻��սô�
	switch (_whereIsWorm)
	{
	case 0:
		if (_isLeft)
		{
			//�̹��� ������ �� �߽��� Ÿ���� �ѱ���� üũ
			if ((_pointx - _moveSpeed)/TILESIZE < (_pointx)/TILESIZE)
			{
				//�Ѱ����� ��ȯ�� ����
				//���� ���ʿ� ���� �ִ��� ���
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���� �ִٸ� �� ���� Ÿ�� �ö󰡰� �ٲ��ش�
					//����� 1��(wormMoveRight)
					_whereIsWorm = 1;

					//x, y���� �����Ѵ�
					//x�� ���� ��(x-1) rect�� �����ʿ��� �̹��� ���ݰ���ŭ ���ϰ�, y�� rect�� �Ʒ��ʿ��� �̼Ӹ�ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� ���� ���µ� �� �Ʒ����� ���� ���� ��� ���� �ڱ� �Ʒ� Ÿ���� Ÿ�� ���������Ѵ�
					//x�� �״��, y�� + 1 ���� �ڱ� �� �Ʒ��� Ÿ�� �˻�, �� �ڱ� �ٷ� �� Ÿ�� �˻�
					//������ Ÿ�� ��������
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� �ڱⰡ ����ִ� �ٴ�(y+1)rect ���ʿ��� �̹��� ���ݰ���ŭ ���ְ�, y�� �ڱⰡ ����ִ� �ٴ�rect ������ �̼Ӹ�ŭ �����ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		else
		{
			//������ �� �߽��� Ÿ���� �ѱ���� üũ, _isLeft�� true�� ���ʹ� �ٸ���
			if ((_pointx + _moveSpeed) > (_pointx)/TILESIZE)
			{
				//���� �Ѱ����� ��ȯ�� ����
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ Ÿ�� �ö󰣴�
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� ���� ��(x+1) rect�� ���ʿ��� �̹��� ���ݰ���ŭ ���ְ�, y�� ���� �� rect�� �Ʒ��ʿ��� �̼Ӹ�ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom - _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy)/TILESIZE + 1, (_pointx + _moveSpeed)/TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� �� ������ üũ�ϰ� �Ʒ��� üũ���ش�
					//�Ѵ� ������ 1��(wormMoveRight)
					_whereIsWorm = 1;
					//������
					//x�� �ڱⰡ ����ִ� �ٴ�(y+1)rect�� �����ʿ��� �̹��� ���ݰ���ŭ �����ְ�, y�� �ڱⰡ ����ִ� �ٴ�rect�� ���ʿ��� �̼Ӹ�ŭ ���Ѵ�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
			}
		}
		break;
	case 1:
		
		if (_isLeft)
		{
			//�߽��� �ѱ���� üũ, �̹��� y���� üũ�Ѵ�
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//�Ѱ����� ��ȯ ����
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ �ö�ź��
					//õ�忡 �����Ƿ� 2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� ���� ��(y-1)rect�� ���ʿ��� �̼Ӹ�ŭ �����ְ�, y�� ���ʺ� rect�� �Ʒ��ʿ��� �̹��� ���ݰ���ŭ �����ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight()/2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//���ʿ��� ���� ���� �� �Ʒ����� ���� ������ ���δ�
					//�ٴڿ� ������ 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ����ִ� �ٴ�(x-1)rect�� �����ʿ��� �̼Ӹ�ŭ ���ְ�, y�� ����ִ� �ٴ�rect�� ���ʿ��� �̹��� ���ݰ���ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//���� �ִ� Ÿ�Ϻ��� ���������� üũ
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� �ִٸ� �����ϴ� 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ��(y+1) rect�� ���ʰ����� �̼Ӹ�ŭ �����ְ�, y�� ��rect�� ���ʿ��� �̹��� ���ݰ���ŭ ���ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE - 1).type != MAPTILE_WALL)
				{
					//�Ʒ��ʿ��� ���� ���� �� ���ʿ��� ������ õ��
					//2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� �ڱⰡ ����ִ� ��(x-1) rect�� �����ʿ��� �̼Ӹ�ŭ ���ְ�, y�� ����ִ� �� rect�� �Ʒ����� �̹��� ���ݰ���ŭ �����ش�
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		
		break;
	case 2:
		//0���� �⺻�� ����
		if (_isLeft)
		{
			//������ �� �߽��� Ÿ���� �ѱ���� üũ, _isLeft�� true�� ���ʹ� �ٸ���
			if ((_pointx + _moveSpeed) / TILESIZE > (_pointx) / TILESIZE)
			{
				//���� �Ѱ����� ��ȯ�� ����
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ Ÿ�� �ö󰣴�
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� ���ʺ�(x+1)�� ���ʿ��� �̹��� ���ݰ���ŭ -, y�� ������ �̼Ӹ�ŭ +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth()/2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx + _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� �� ������ üũ�ϰ� �Ʒ��� üũ���ش�
					//�Ѵ� ������ 1��(wormMoveRight)
					_whereIsWorm = 1;
					//������
					//x�� �ڱⰡ ������(y - 1)�� �����ʿ��� �̹�������+, y�� �ڱⰡ �������� �Ʒ����� �̼Ӹ�ŭ ����
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom - _moveSpeed;
				}
			}
		}
		else
		{
			//�̹��� ������ �� �߽��� Ÿ���� �ѱ���� üũ
			if ((_pointx - _moveSpeed) / TILESIZE < (_pointx) / TILESIZE)
			{
				//�Ѱ����� ��ȯ�� ����
				//���� ���ʿ� ���� �ִ��� ���
				if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type == MAPTILE_WALL)
				{
					//���� �ִٸ� �� ���� Ÿ�� �ö󰡰� �ٲ��ش�
					//����� 1��(wormMoveRight)
					_whereIsWorm = 1;

					//x, y���� �����Ѵ�
					//x�� ���ʺ�(x-1)�� �����ʿ��� �̹��� ����+, y�� ���ʺ� ������ �̼Ӹ�ŭ +
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.right + IMAGEMANAGER->findImage("wormMoveRight")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE - 1).rc.top + _moveSpeed;
				}
				else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx - _moveSpeed) / TILESIZE).type != MAPTILE_WALL)
				{
					//���� ���ʿ� ���� ���µ� �� �Ʒ����� ���� ���� ��� ���� �ڱ� �Ʒ� Ÿ���� Ÿ�� ���������Ѵ�
					//x�� �״��, y�� + 1 ���� �ڱ� �� �Ʒ��� Ÿ�� �˻�, �� �ڱ� �ٷ� �� Ÿ�� �˻�
					//������ Ÿ�� ��������
					//3��(wormMoveLeft)
					_whereIsWorm = 3;
					//������
					//x�� ������(y-1)���ʿ��� �̹������ݸ�ŭ-, y�� ������ �Ʒ��� �̼Ӹ�ŭ-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.left - IMAGEMANAGER->findImage("wormMoveLeft")->getFrameWidth() / 2;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + _moveSpeed;
				}
			}
		}
		break;
	case 3:
		if (_isLeft)
		{
			//���� �ִ� Ÿ�Ϻ��� ���������� üũ
			if ((_pointy + _moveSpeed) / TILESIZE > (_pointy)/TILESIZE)
			{
				if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� �ִٸ� �����ϴ� 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ���ʺ�(y+1)�� �����ʿ��� �̼Ӹ�ŭ-, y�� ������ �̹������ݸ�ŭ-
					_pointx = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE + 1, (_pointx) / TILESIZE).rc.top - IMAGEMANAGER->findImage("wormMoveUp")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy + _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//�Ʒ��ʿ��� ���� ���� �� ���ʿ��� ������ õ��
					//2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� ������(x + 1)���ʿ��� �̼Ӹ�ŭ+, y�� �Ʒ����� �̹��� ���ݸ�ŭ+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.left + _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE + 1).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
			}
		}
		else
		{
			//�߽��� �ѱ���� üũ, �̹��� y���� üũ�Ѵ�
			if ((_pointy - _moveSpeed)/TILESIZE < (_pointy)/TILESIZE)
			{
				//�Ѱ����� ��ȯ ����
				if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
				{
					//���ʿ� ���� ������ �ö�ź��
					//õ�忡 �����Ƿ� 2��(wormMoveDown)
					_whereIsWorm = 2;
					//������
					//x�� ���ʺ�(y-1)�� �����ʿ��� �̼Ӹ�ŭ-, y�� �Ʒ����� �̹��� ���ݸ�ŭ+
					_pointx = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.right - _moveSpeed;
					_pointy = _map->getMapInfo((_pointy) / TILESIZE - 1, (_pointx) / TILESIZE).rc.bottom + IMAGEMANAGER->findImage("wormMoveDown")->getFrameHeight() / 2;
				}
				else if (_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE).type != MAPTILE_WALL &&
					_map->getMapInfo((_pointy - _moveSpeed) / TILESIZE, (_pointx) / TILESIZE + 1).type != MAPTILE_WALL)
				{
					//���ʿ��� ���� ���� �� �Ʒ����� ���� ������ ���δ�
					//�ٴڿ� ������ 0��(wormMoveUp)
					_whereIsWorm = 0;
					//������
					//x�� ������(x+1)�� ���ʿ��� �̼Ӹ�ŭ+, y�� ������ �̹��� ���ݸ�ŭ -
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
	//��Ȳ�� ���� �ٸ���
	switch (_whereIsWorm)
	{
	case 0:
		//�ٴڿ� �پ�������, ���߿� ���ִ����� �ٴڿ� �پ��ִ����� Ȯ���Ѵ�
		//���� �ٴ����� Ȯ��(_moveUp)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//���� �ִ� Ÿ���� ���� ��� �ٴڿ� �پ��ִ� ��츦 å���ϹǷ� �����ִ� Ÿ�� ���� �÷��ش�
			//x�� �������� y�� �÷��ָ� ��
			_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.top - _moveUp->getFrameHeight() / 2;
		}
		else if (_map->getMapInfo((_pointy + _moveUp->getFrameHeight()/2 + 1)/TILESIZE, (_pointx)/TILESIZE).type != MAPTILE_WALL)
		{
			//���� ���� Ÿ����ǥ�� �ڱ� �Ʒ� Ÿ����ǥ�� ���ٸ� ���߿� �������Ƿ� ��ġ�� �����ش�
			_pointy += 5;
		}
		break;
	case 1:
		//�ٴڿ� �پ����������� üũ, �̰� ���ʺ��� �پ���������(_moveRight)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//x�� �ٲ��ָ� ��
			_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.right + _moveRight->getFrameWidth() / 2;
		}
		else if (_map->getMapInfo((_pointy)/TILESIZE, (_pointx - _moveRight->getFrameWidth()/2 - 1)/TILESIZE).type != MAPTILE_WALL)
		{
			//���� x�� �ٲ��ָ� ��
			_pointx -= 5;
		}
		break;
	case 2:
		//õ��(_moveDown)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//y�� ����, �ٴڿ� �پ��������ʹ� �ݴ��
			_pointy = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.bottom + _moveDown->getFrameHeight() / 2;
		}
		else if (_map->getMapInfo((_pointy - _moveDown->getFrameHeight()/2 - 1)/TILESIZE, (_pointx)/TILESIZE).type != MAPTILE_WALL)
		{
			//y�� ����, �ٴڿ� �پ��������ʹ� �ݴ��
			_pointy -= 5;
		}
		break;
	case 3:
		//������ ��(_moveLeft)
		if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).type == MAPTILE_WALL)
		{
			//x�� ����, �޺��� �ݴ�
			_pointx = _map->getMapInfo((_pointy) / TILESIZE, (_pointx) / TILESIZE).rc.left - _moveLeft->getFrameWidth() / 2;
		}
		else if (_map->getMapInfo((_pointy) / TILESIZE, (_pointx + _moveLeft->getFrameWidth() / 2 + 1) / TILESIZE).type != MAPTILE_WALL)
		{
			//x�� ����, �޺��� �ݴ�
			_pointx += 5;
		}
		break;
	}
}