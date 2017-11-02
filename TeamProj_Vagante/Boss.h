#pragma once
#include "gameNode.h"
#include <list>
#include <algorithm>
#include "Map.h"
#include "vaganteStructEnum.h"
#include "FireBall.h"

/*
!vaganteStructEnum.h에서 선언했으니 참조만 할 것!
enum BossSTATE {
BossSTATE_IDLE,
BossSTATE_MOVING,
BossSTATE_ATTACKING,
BossSTATE_HIT,
BossSTATE_JUMPING,
BossSTATE_FALLING,
BossSTATE_DEAD
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
*/



enum BOSSSTATE {
	BOSSSTATE_SLEEP,
	BOSSSTATE_ACTIVATE,
	BOSSSTATE_FLYING,
	BOSSSTATE_FIREING,
	BOSSSTATE_STAMPING,
	BOSSSTATE_DEAD
};

//For A*알고리즘!!
typedef struct vertex {
	int f;		//비용
	int h;		//이동비용
	int g;		//예상이동비용
	int vx, vy;	//위치
	vertex *p;	//부모 vertex
	bool operator<(const vertex &v) const {
		return (f > v.f);
	}

};
class Player;
class UI;
class Boss : public gameNode
{
private:
	image* _image;														//이미지
	BOSSSTATE _state;													//상태
	Player* _player;													//플레이어 정보
	UI* _ui;															//ui
	tagStatusEffect _statusEffect[5];									//상태이상
	tagStat _statistics;												//스탯
	int _currentFrameX, _currentFrameY;									//프레임
	RECT _rc;															//피격범위
	float _pointx, _pointy;												//좌표
	float _xspeed, _yspeed;												//x,y축 이동 속도
	int _money;															//몬스터 죽으면 나올 동전 갯수
	Map* _map;															//맵 정보
	bool _isFindPlayer;													//플레이어를 발견한 상태인지
	FireBall* _fireball;

	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;// 위치 정보

	bool _canfire;
	bool _lookleft;
	float _actTimer;													//행동시간
	int _frameTime, _frameFPS;											//프레임 변화용
	float _minCog, _maxCog;												//몬스터 최초 인식범위, 한계 인식범위
	float _timerForFrameUpdate;

	//For A* 알고리즘!!
	int _curTileX, _curTileY;		//현재 위치
	int _goalTileX, _goalTileY;		//목표 위치
	vertex _startpoint;
	int _tileinfo[40][58];			//맵 정보

	//일단 벡터로 만들어봄
	vector<vertex> _openlist;
	vector<vertex> _closelist;
	void add_openlist(vertex v);	//열림목록에 추가
	void add_closelist(vertex v);	//닫힘목록에 추가
	vertex pop_openlist();			//열림목록에서 최소값 리턴
	vertex pop_closelist();			//닫힘목록에서 최소값 리턴
	vertex calc_vertex(vertex v);	//vertex의 값 계산
	void astar();

public:
	HRESULT init();
	virtual HRESULT init(POINT point);
	void release();
	void update();
	void render();
	void render(POINT camera);
	void draw(POINT camera);

	void stateHandle();
	void speedAdjust();
	void fireFireBall();
	void imageChange();

	virtual void move();			// 이동관련함수
	void mapCollisionHandle();
	virtual void frameUpdate();		// 프레임 업데이트


									//공격 받았을 시 (데미지만)
	void getDamaged(int damage) { _statistics.hp -= damage; }
	//공격 받았을 시 (데미지&넉백)
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _xspeed += cosf(angle)*knockbackpower; _yspeed -= sinf(angle)*knockbackpower; }
	//상태이상
	void addStatusEffect(tagStatusEffect statuseffect);


	//설정자&접근자
	int getHP() { return _statistics.hp; }
	void setHP(int hp) { _statistics.hp = hp; }
	tagStat getStat() { return _statistics; }
	void setStat(tagStat stat) { _statistics = stat; }
	RECT getRect() { return _rc; }
	POINT getPoint() { return PointMake(_pointx, _pointy); }
	void setPoint(POINT point) { _pointx = point.x; _pointy = point.y; }
	int getMoney() { return _money; }
	float getXSpeed() { return _xspeed; }
	void setXSpeed(float xspeed) { _xspeed = xspeed; }
	float getYSpeed() { return _yspeed; }
	void setYSpeed(float yspeed) { _yspeed = yspeed; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUiAddressLink(UI* ui) { _ui = ui; }
	void setMapAddressLink(Map* map) { _map = map; }


	void statusEffect();

	Boss();
	~Boss();
};

