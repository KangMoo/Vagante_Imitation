#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

class EnemyManager;
class Map;
class UI;

#define JUMPPOWERSTART 5
#define JUMPPOWERMAX 10
#define FALLPOWERMAX 8
#define RUNPOWERSTART 1
#define RUNPOWERMAX 3
#define DASHPOWER 8
#define DASHTIME 2.5
#define LADDERUPSPEED 2
#define LADDERDOWNSPEED -3

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
	typedef vector<MYRECT> vRange;
	typedef vector<MYRECT>::iterator viRange;

	tagPlayerInfo _player;
	EnemyManager* _em;
	Map* _map;
	UI* _ui;
	bool _canCtrl;		//���� ���� ���� �Ǵ�


	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;// ��ġ ����
	int _curTileX, _curTileY; // ���� Ÿ�� ��ġ
	int _prevTileX, _prevTileY; // ���� Ÿ�� ��ġ

	vRange _vHitRange;		//�ǰ� ���� ��Ʈ
	vRange _vAttackRange;	//���ݹ�����Ʈ�� ���� ���� (���ݹ����� �������� �����츦 ���)



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
	void holdLadder();			// ��ٸ� �Ŵ޸���
	void canDown();			// ��ٸ� �Ŵ޸���

	//���� �޾��� �� (��������)
	void getDamaged(int damage) { _player.stat.hp -= damage; }
	//���� �޾��� �� (������&�˹�)
	void getDamaged(int damage, float angle, float knockbackpower) { _player.stat.hp -= damage; _player.xspeed += cosf(angle)*knockbackpower; _player.yspeed -= sinf(angle)*knockbackpower; }
	//�����̻�ȿ�� �߰�
	void addStatusEffect(tagStatusEffect statuseffect);

	//����&������
	inline int getHP() { return _player.stat.hp; }
	inline void setHP(int hp) { _player.stat.hp = hp; }
	inline POINT getPoint() { return PointMake(int(_player.pointx), int(_player.pointy)); }
	inline void setPoint(POINT point) { _player.pointx = point.x; _player.pointy = point.y; }
	inline RECT getRect() { return _player.rc; }
	inline PLAYERSTATE getState() { return _player.state; }
	inline void setState(PLAYERSTATE state) { _player.state = state; }
	inline int getMoney() { return _player.money; }
	inline void setMoney(int money) { _player.money = money; }
	inline tagStat getStat() { return _player.stat; }
	inline void setStat(tagStat stat) { _player.stat = stat; }
	inline bool getCtrl() { return _canCtrl; }
	inline void setCtrl(bool ctrl) { _canCtrl = ctrl; }
	inline float getXSpeed() { return _player.xspeed; }
	inline void setXSpeed(float xspeed) { _player.xspeed = xspeed; }
	inline float getYSpeed() { return _player.yspeed; }
	inline void setYSpeed(float yspeed) { _player.yspeed = yspeed; }

	inline vRange getVHitRange(void) { return _vHitRange; }
	inline vRange getVAttackRange(void) { return _vAttackRange; }


	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }
	void setUiAddressLink(UI* ui) { _ui = ui; }
	Player();
	~Player();
};

