#pragma once
#include "gameNode.h"
#include <vector>
#include "vaganteStructEnum.h"

class Player;
class Map;
class UI;
struct tagFireBall
{

	RECT rc;						//미싸일 충돌박스
	float x, y;						//미싸일 좌표
	float angle;					//미쏴일 앵글
	float radius;					//반지름
	float speed;					//속도
	float fireX, fireY;				//미싸일 발사시점 좌표
	bool fire;						//쐈어?
	int currentFrameX, currentFrameY;
	int count;
	float power;
	int type;
};

//공용으로 사용하는 총알
class FireBall : public gameNode
{
private:
	vector<tagFireBall> _vFireBall;
	vector<tagFireBall>::iterator _viFireBall;

	image* _fireBallImage;			//미싸일 이미지
	const char* _imageName;
	float _range;
	int _FireBallMax;
	float _timerForFrameUpdate;
	Player* _player;
	Map* _map;
	UI* _ui;

public:
	HRESULT init(int FireBallMax, float range, const char* imageName);
	void release(void);
	void update(void);
	void render(POINT camera);
	void frameHandle();

	void fire(float x, float y, float angle, float speed);

	void move(void);
	void hitPlayer();
	void removeFireBall(int arrNum);
	vector<tagFireBall> getVFireBall(void) { return _vFireBall; }
	vector<tagFireBall>::iterator getVIFireBall(void) { return _viFireBall; }
	void setMapAddressLink(Map* map) { _map = map; }
	void setPlayerAddressLink(Player* player) { _player = player; }
	void setUIAddressLink(UI* ui) { _ui = ui; }

	//for A* 알고리즘
	FireBall();
	~FireBall();
};
