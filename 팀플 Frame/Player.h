#pragma once
#include "gameNode.h"

class EnemyManager;
class Map;

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
//상태이상 종류
enum STATUSEFFECT{
	STATUSEFFECT_IDLE,
	STATUSEFFECT_POISON,
	STATUSEFFECT_FIRE,
	STATUSEFFECT_STUN,
};
//상태이상 구조체
struct tagStatusEffect {
	//상태이상 종류
	STATUSEFFECT type;
	//남은 시간
	int leftTime;
	//상태이상 데미지
	int damage;
};

struct tagPlayerInfo {
	image* image;
	PLAYERSTATE state;
	tagStatusEffect statusEffect;
	float pointx, pointy;				//현재좌표
	float xspeed;						//현재 좌우 이동 속도
	float yspeed;						//현재 상하 이동 속도
	bool lookingRight;					//보고있는 방향
	bool lookingLeft;
	int hp;								//체력
	float atkspeed;						//공속
	float jumppower;					//점프력
	float speed;						//스피드
	RECT rc;
	int money;							//돈
	float gravity;						//중력
	int currentFrameX, curretFrameY;	//현재 프레임
};
class Player : public gameNode
{
private:
	tagPlayerInfo _player;
	EnemyManager* _em;
	Map* _map;
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
	void collisionCheck();	// 지형충돌체크
	void jump();			// 점프
	void attack();			// 공격

	//공격 받았을 시
	void getDamaged(int damage) { _player.hp -= damage; }

	//설정&접근자
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

	//넉백을 위한 x,yspeed 설정&접근자
	float getXSpeed() { return _player.xspeed; }
	void setXSpeed(float xspeed) { _player.xspeed = xspeed; }
	float getYSpeed() { return _player.yspeed; }
	void setYSpeed(float yspeed) { _player.yspeed = yspeed; }


	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }
	Player();
	~Player();
};

