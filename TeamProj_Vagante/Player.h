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
!vaganteStructEnum.h 선언했으니 참조만 할 것!
enum PLAYERSTATE {
	PLAYERSTATE_ILDE,				//일반상태
	PLAYERSTATE_LOOKING_UP,			//위를 올려다보는 상태
	PLAYERSTATE_LOOKING_DOWN,		//아래를 내려다보는 상태
	PLAYERSTATE_MOVING,				//이동
	PLAYERSTATE_JUMPING,			//점프
	PLAYERSTATE_FALLING,			//추락
	PLAYERSTATE_ATTACKING_1,		//공격1
	PLAYERSTATE_ATTACKING_2,		//공격2
	PLAYERSTATE_ATTACKING_3,		//공격3
	PLAYERSTATE_HIT,				//공격받음
	PLAYERSTATE_HOLDING_WALL,		//벽 매달림
	PLAYERSTATE_HOLDING_LADDER,		//사다리 매달림
	PLAYERSTATE_CHEKINGINVENTORY	//인벤토리 확인중
};
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
};
*/
class Player : public gameNode
{
private:
	tagPlayerInfo _player;
	EnemyManager* _em;
	Map* _map;
	UI* _ui;
	bool _canCtrl;		//조작 가능 여부 판단

public:
	HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void move();			// 이동관련함수
	void keyintput();		// 키 입력 함수
	void setmaptileInfo();	// 타일값 얻어오기
	void jump();			// 점프
	void attack();			// 공격

	//공격 받았을 시 (데미지만)
	void getDamaged(int damage) { _player.stat.hp -= damage; }
	//공격 받았을 시 (데미지&넉백)
	void getDamaged(int damage, float angle, float knockbackpower) { _player.stat.hp -= damage; _player.xspeed += cosf(angle)*knockbackpower; _player.yspeed -= sinf(angle)*knockbackpower; }
	//상태이상효과 추가
	void addStatusEffect(tagStatusEffect statuseffect);

	//설정&접근자
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

