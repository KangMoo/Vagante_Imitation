#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

class EnemyManager;
class Map;
class UI;

#define JUMPPOWERSTART 7
#define JUMPPOWERMAX 10
#define FALLPOWERMAX 10
#define RUNPOWERSTART 1
#define RUNPOWERMAX 3
#define DASHPOWER 8
#define DASHTIME 2.5
#define HANGSPEED 2

/*
!vaganteStructEnum.h ���������� ������ �� ��!
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

struct tagPlayerInfo {
	image* image;
	PLAYERSTATE state;
	tagStatusEffect statusEffect[5];	//�����̻�
	tagStat stat;						//����
	float pointx, pointy;				//������ǥ
	float xspeed;						//���� �¿� �̵� �ӵ�
	float yspeed;						//���� ���� �̵� �ӵ�
	bool lookingRight;					//�����ִ� ����
	bool lookingLeft;					//�����ִ� ����
	float jumppower;					//������
	RECT rc;							//��Ʈ~!
	int money;							//��
	float gravity;						//�߷�
	int currentFrameX, curretFrameY;	//���� ������
};
*/
class Player : public gameNode
{
private:
	tagPlayerInfo _player;
	EnemyManager* _em;
	Map* _map;
	UI* _ui;
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
	void setmaptileInfo();	// Ÿ�ϰ� ������
	void jump();			// ����
	void attack();			// ����

	//���� �޾��� �� (��������)
	void getDamaged(int damage) { _player.stat.hp -= damage; }
	//���� �޾��� �� (������&�˹�)
	void getDamaged(int damage, float angle, float knockbackpower) { _player.stat.hp -= damage; _player.xspeed += cosf(angle)*knockbackpower; _player.yspeed -= sinf(angle)*knockbackpower; }
	//�����̻�ȿ�� �߰�
	void addStatusEffect(tagStatusEffect statuseffect);

	//����&������
	int getHP() { return _player.stat.hp; }
	void setHP(int hp) { _player.stat.hp = hp; }
	POINT getPoint() { return PointMake(int(_player.pointx), int(_player.pointy)); }
	void setPoint(POINT point) { _player.pointx = point.x; _player.pointy = point.y; }
	RECT getRect() { return _player.rc; }
	PLAYERSTATE getState() { return _player.state; }
	void setState(PLAYERSTATE state) { _player.state = state; }
	int getMoney() { return _player.money; }
	void setMoney(int money) { _player.money = money; }
	tagStat getStat() { return _player.stat; }
	void setStat(tagStat stat) { _player.stat = stat; }
	bool getCtrl() { return _canCtrl; }
	void setCtrl(bool ctrl) { _canCtrl = ctrl; }
	float getXSpeed() { return _player.xspeed; }
	void setXSpeed(float xspeed) { _player.xspeed = xspeed; }
	float getYSpeed() { return _player.yspeed; }
	void setYSpeed(float yspeed) { _player.yspeed = yspeed; }


	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }
	void setUiAddressLink(UI* ui) { _ui = ui; }
	Player();
	~Player();
};

