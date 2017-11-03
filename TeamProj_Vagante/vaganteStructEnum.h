#pragma once
#define TILESIZE 32
#define STATUSEFFECTMAX 5
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

//�����̻� ����
enum STATUSEFFECT {
	STATUSEFFECT_NULL,		//�����̻� ����
	STATUSEFFECT_POISON,	//�����̻� : ��
	STATUSEFFECT_FIRE,		//�����̻� : ��
	STATUSEFFECT_STUN,		//�����̻� : ����
	STATUSEFFECT_HEAL,		//�����̻� : ȸ��
};
//�����̻� ����ü
struct tagStatusEffect {
	//�����̻� ����
	STATUSEFFECT type;
	//���� ���� �ð�
	int leftTime;
	//�����̻� ������ (ȸ���� ��� ġ����)
	int damage;
};
enum ITEMSTATE {
	ITEMSTATE_ONMAP,		//�ʿ� ����
	ITEMSTATE_INVENTORY,	//�κ��丮�� ����
	ITEMSTATE_EQUIP,		//�����
	ITEMSTATE_NONEXSIST		//�������� ����
};
struct tagItemInfo {
	image* bigImg;			//������ �̹���(ū��)
	image* smallImg;		//������ �̹���(������)
	POINT point;			//������ ��ǥ(�ʿ� ���� ��)
	RECT rc;				//������ ��Ʈ(�ʿ� ���� �� ���)
	tagStat itemstat;		//������ �ɷ�ġ
	int itemstate;			//���� �������� ����(��,�κ��丮,���������� Ȯ��)
};

enum ENEMYSTATE {
	ENEMYSTATE_IDLE,
	ENEMYSTATE_MOVING,
	ENEMYSTATE_ATTACKING,
	ENEMYSTATE_HIT,
	ENEMYSTATE_JUMPING,
	ENEMYSTATE_FALLING,
	ENEMYSTATE_DEAD
};
enum MAPTILETYPE {
	MAPTILE_NULL,
	MAPTILE_WALL,
	MAPTILE_WALL2,
	MAPTILE_LADDER,
	MAPTILE_GROUND_CAN_GO_DOWN_1,
	MAPTILE_SPIKE_TRAP
};
struct mapInfo {
	//Ÿ�� ����
	int type;
	//Ÿ�� ��ġ
	POINT point;
	//Ÿ�� ��Ʈ
	RECT rc;
};
enum PLAYERSTATE {
	PLAYERSTATE_IDLE,				//�Ϲݻ���
	PLAYERSTATE_MOVING,				//�̵�
	PLAYERSTATE_LOOKING_DOWN,		//�Ʒ��� �����ٺ��� ����
	PLAYERSTATE_LOOKING_DOWN_MOVING,//�Ʒ��� �����ٺ��� �����̴� ����
	PLAYERSTATE_JUMPING,			//����
	PLAYERSTATE_FALLING,			//�߶�
	PLAYERSTATE_ATTACKING,			//���� ����
	PLAYERSTATE_ATTACKING_JUMP,		//���� ����
	PLAYERSTATE_ATTACKING_3,		//����3
	PLAYERSTATE_HIT,				//���ݹ���
	PLAYERSTATE_HOLDING_WALL,		//�� �Ŵ޸�
	PLAYERSTATE_HOLDING_LADDERUP,		//��ٸ� �Ŵ޸� �ö�
	PLAYERSTATE_HOLDING_LADDERDOWN,		//��ٸ� �Ŵ޸� ������
	PLAYERSTATE_CHEKINGINVENTORY	//�κ��丮 Ȯ����
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
	
	tagItemInfo equipWeapon;
};

typedef vector<MYRECT> vAttackRange;
typedef vector<MYRECT>::iterator viAttackRange;
