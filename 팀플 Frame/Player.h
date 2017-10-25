#pragma once
#include "gameNode.h"

class EnemyManager;
class Map;

enum PLAYERSTATE {
	PLAYERSTATE_ILDE,				//�Ϲݻ���
	PLAYERSTATE_LOOKING_UP,			//���� �÷��ٺ��� ����
	PLAYERSTATE_LOOKING_DOWN,		//�Ʒ��� �����ٺ��� ����
	PLAYERSTATE_MOVING,				//�̵�
	PLAYERSTATE_JUMPING,			//����
	PLAYERSTATE_FALLING,			//�߶�
	PLAYERSTATE_ATTACKING_1,		//����1
	PLAYERSTATE_ATTACKING_2,		//����2
	PLAYERSTATE_ATTACKING_3,		//����3
	PLAYERSTATE_HIT,				//���ݹ���
	PLAYERSTATE_HOLDING_WALL,		//�� �Ŵ޸�
	PLAYERSTATE_HOLDING_LADDER,		//��ٸ� �Ŵ޸�
	PLAYERSTATE_CHEKINGINVENTORY	//�κ��丮 Ȯ����
};
//�����̻� ����
enum STATUSEFFECT{
	STATUSEFFECT_IDLE,
	STATUSEFFECT_POISON,
	STATUSEFFECT_FIRE,
	STATUSEFFECT_STUN,
};
//�����̻� ����ü
struct tagStatusEffect {
	//�����̻� ����
	STATUSEFFECT type;
	//���� �ð�
	int leftTime;
	//�����̻� ������
	int damage;
};

struct tagPlayerInfo {
	image* image;
	PLAYERSTATE state;
	tagStatusEffect statusEffect;
	float pointx, pointy;				//������ǥ
	float xspeed;						//���� �¿� �̵� �ӵ�
	float yspeed;						//���� ���� �̵� �ӵ�
	bool lookingRight;					//�����ִ� ����
	bool lookingLeft;
	int hp;								//ü��
	float atkspeed;						//����
	float jumppower;					//������
	float speed;						//���ǵ�
	RECT rc;
	int money;							//��
	float gravity;						//�߷�
	int currentFrameX, curretFrameY;	//���� ������
};
class Player : public gameNode
{
private:
	tagPlayerInfo _player;
	EnemyManager* _em;
	Map* _map;
	bool _canCtrl;		//���� ���� ���� �Ǵ�

public:
	HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void move();			// �̵������Լ�
	void keyintput();		// Ű �Է� �Լ�
	void collisionCheck();	// �����浹üũ
	void jump();			// ����
	void attack();			// ����

	//���� �޾��� ��
	void getDamaged(int damage) { _player.hp -= damage; }

	//����&������
	int getHP() { return _player.hp; }
	void setHP(int hp) { _player.hp = hp; }
	void setStatusEffect(tagStatusEffect statuseffect) { _player.statusEffect = statuseffect; }
	POINT getPoint() { return PointMake(int(_player.pointx), int(_player.pointy)); }
	void setPoint(POINT point) { _player.pointx = point.x; _player.pointy = point.y; }
	RECT getRect() { return _player.rc; }
	PLAYERSTATE getState() { return _player.state; }
	void setState(PLAYERSTATE state) { _player.state = state; }
	int getMoney() { return _player.money; }
	void setMoney(int money) { _player.money = money; }
	bool getCtrl() { return _canCtrl; }
	void setCtrl(bool ctrl) { _canCtrl = ctrl; }

	//�˹��� ���� x,yspeed ����&������
	float getXSpeed() { return _player.xspeed; }
	void setXSpeed(float xspeed) { _player.xspeed = xspeed; }
	float getYSpeed() { return _player.yspeed; }
	void setYSpeed(float yspeed) { _player.yspeed = yspeed; }


	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }
	Player();
	~Player();
};

