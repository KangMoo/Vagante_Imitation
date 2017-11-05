#pragma once
#include "gameNode.h"
#include <list>
#include <algorithm>
#include "Map.h"
#include "vaganteStructEnum.h"
#include "FireBall.h"
#include "UI.h"

/*
!vaganteStructEnum.h���� ���������� ������ �� ��!
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



enum BOSSSTATE {
	BOSSSTATE_SLEEP,
	BOSSSTATE_ACTIVATE,
	BOSSSTATE_FLYING,
	BOSSSTATE_FIREING,
	BOSSSTATE_STAMPING,
	BOSSSTATE_DEAD
};

//For A*�˰���!!
typedef struct vertex {
	int f;		//���
	int h;		//�̵����
	int g;		//�����̵����
	int vx, vy;	//��ġ
	int px, py;	//�θ� ��ġ
	//vertex *p;	//�θ� vertex
	bool operator<(const vertex &v) const {
		return (f > v.f);
	}
};
class Player;
class UI;
class Boss : public gameNode
{
private:
	image* _image;														//�̹���
	BOSSSTATE _state;													//����
	Player* _player;													//�÷��̾� ����
	UI* _ui;															//ui
	tagStatusEffect _statusEffect[5];									//�����̻�
	tagStat _statistics;												//����
	int _currentFrameX, _currentFrameY;									//������
	RECT _rc;															//�̵��浹����
	RECT _rcHit;														//�����浹���� add by JW
	float _pointx, _pointy;												//��ǥ
	float _xspeed, _yspeed;												//x,y�� �̵� �ӵ�
	int _money;															//���� ������ ���� ���� ����
	Map* _map;															//�� ����
	bool _isFindPlayer;													//�÷��̾ �߰��� ��������
	bool _stampHitLand;													//stamp���� üũ�� ���� bool��	
	bool _totallydead;													//���� ���� Ȯ���� ���� bool��
	int _fireballCount;													//�� �߻� ����
	FireBall* _fireball;
	mapInfo upL, upM, upR, midL, midM, midR, botL, botM, botR;			// ��ġ ����
	bool _canfire;
	bool _lookleft;
	float _actTimer;													//�ൿ�ð�
	int _frameTime, _frameFPS;											//������ ��ȭ��
	float _minCog, _maxCog;												//���� ���� �νĹ���, �Ѱ� �νĹ���
	float _timerForFrameUpdate;
	float _timerForAstar;
	//For A* �˰���!!
	int _curTileX, _curTileY;		//���� ��ġ
	int _goalTileX, _goalTileY;		//��ǥ ��ġ
	vertex _startpoint;			//������
	vertex* _currentvertex;			//���� Ž�� ��ġ
	vertex _npcurrentvertex;			//���� Ž�� ��ġ
	vector<vertex> _wayToPlayer;		//�÷��̾�� ���� �� ����
	int _tileinfo[40][58];			//�� ����
	//�ϴ� ���ͷ� ����
	vector<vertex> _openlist;
	vector<vertex> _closelist;
	void add_openlist(vertex v);					//������Ͽ� �߰�
	void add_closelist(vertex v);					//������Ͽ� �߰�
	vertex getcloselist(int x, int y);				//��������� �׸� ��������
	vertex getopenlist(int x, int y);				//��������� �׸� ��������
	vertex pop_openlist();							//������Ͽ��� �ּҰ� ����
	vertex pop_closelist();						//������Ͽ��� �� �� ����
	vertex pop_closelist(int vx, int vy);			//�ش��ϴ� ����� vertex�� ����
	vertex calc_vertex(vertex v, vertex p);					//vertex�� �� ���
	void add_opelistEightWay(vertex* v);	//���� 8�� Ÿ�� ������Ͽ� �߰�
	bool checkGoal();
	void makeWay();
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
	void stamping();
	void imageChange();
	void deadCheck();

	virtual void move();			// �̵������Լ�
	void mapCollisionHandle();
	virtual void frameUpdate();		// ������ ������Ʈ


									//���� �޾��� �� (��������)
	void getDamaged(int damage) { _statistics.hp -= damage; SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1);}
	//���� �޾��� �� (������&�˹�)
	void getDamaged(int damage, float angle, float knockbackpower) { _statistics.hp -= damage; _ui->hitOutput(_pointx, _pointy, damage, LETTER_WHITE);  SOUNDMANAGER->play("5_Enemy_Demage_Sound", 1); }
	//�����̻�
	void addStatusEffect(tagStatusEffect statuseffect);


	//������&������
	int getHP() { return _statistics.hp; }
	void setHP(int hp) { _statistics.hp = hp; }
	tagStat getStat() { return _statistics; }
	void setStat(tagStat stat) { _statistics = stat; }
	RECT getRect() { return _rcHit; }
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

