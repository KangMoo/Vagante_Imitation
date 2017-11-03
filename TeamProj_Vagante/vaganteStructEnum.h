#pragma once
#define TILESIZE 32
#define STATUSEFFECTMAX 5
struct tagStat {
	int hp;		//체력
	int str;	//힘
	int dex;	//민첩
	int vit;	//활력
	int inl;	//지능
	int lck;	//운
	int def;	//방어력
	int fir;	//불저항
	int ice;	//얼음저항
	int lgt;	//빛저항
	int psn;	//독저항
	int mel;	//근접공격데미지
	int rng;	//활공격데미지
	int crit;	//크리티컬확률
	int aspd;	//공속
	int spd;	//이속
};

//상태이상 종류
enum STATUSEFFECT {
	STATUSEFFECT_NULL,		//상태이상 없음
	STATUSEFFECT_POISON,	//상태이상 : 독
	STATUSEFFECT_FIRE,		//상태이상 : 불
	STATUSEFFECT_STUN,		//상태이상 : 스턴
	STATUSEFFECT_HEAL,		//상태이상 : 회복
};
//상태이상 구조체
struct tagStatusEffect {
	//상태이상 종류
	STATUSEFFECT type;
	//남은 지속 시간
	int leftTime;
	//상태이상 데미지 (회복일 경우 치유량)
	int damage;
};
enum ITEMSTATE {
	ITEMSTATE_ONMAP,		//맵에 있음
	ITEMSTATE_INVENTORY,	//인벤토리에 있음
	ITEMSTATE_EQUIP,		//장비중
	ITEMSTATE_NONEXSIST		//존재하지 않음
};
struct tagItemInfo {
	image* bigImg;			//아이템 이미지(큰거)
	image* smallImg;		//아이템 이미지(작은거)
	POINT point;			//아이템 좌표(맵에 있을 시)
	RECT rc;				//아이템 렉트(맵에 있을 시 사용)
	tagStat itemstat;		//아이템 능력치
	int itemstate;			//현재 아이템의 상태(맵,인벤토리,착용중인지 확인)
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
	//타일 종류
	int type;
	//타일 위치
	POINT point;
	//타일 렉트
	RECT rc;
};
enum PLAYERSTATE {
	PLAYERSTATE_IDLE,				//일반상태
	PLAYERSTATE_MOVING,				//이동
	PLAYERSTATE_LOOKING_DOWN,		//아래를 내려다보는 상태
	PLAYERSTATE_LOOKING_DOWN_MOVING,//아래를 내려다보고 움직이는 상태
	PLAYERSTATE_JUMPING,			//점프
	PLAYERSTATE_FALLING,			//추락
	PLAYERSTATE_ATTACKING,			//지상 공격
	PLAYERSTATE_ATTACKING_JUMP,		//점프 공격
	PLAYERSTATE_ATTACKING_3,		//공격3
	PLAYERSTATE_HIT,				//공격받음
	PLAYERSTATE_HOLDING_WALL,		//벽 매달림
	PLAYERSTATE_HOLDING_LADDERUP,		//사다리 매달림 올라감
	PLAYERSTATE_HOLDING_LADDERDOWN,		//사다리 매달림 내려감
	PLAYERSTATE_CHEKINGINVENTORY	//인벤토리 확인중
};
struct tagPlayerInfo {
	image* image;
	PLAYERSTATE state;
	tagStatusEffect statusEffect[5];	//상태이상
	tagStat stat;						//스탯
	float pointx, pointy;				//현재좌표
	float xspeed;						//현재 좌우 이동 속도
	float yspeed;						//현재 상하 이동 속도
	bool lookingRight;					//보고있는 방향
	bool lookingLeft;					//보고있는 방향
	float jumppower;					//점프력
	RECT rc;							//렉트~!
	int money;							//돈
	float gravity;						//중력
	int currentFrameX, curretFrameY;	//현재 프레임
	
	tagItemInfo equipWeapon;
};

typedef vector<MYRECT> vAttackRange;
typedef vector<MYRECT>::iterator viAttackRange;
