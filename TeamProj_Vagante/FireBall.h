#pragma once
#include "gameNode.h"
#include <vector>
#include "vaganteStructEnum.h"

class Player;
class Map;
class UI;
struct tagFireBall
{

	RECT rc;						//�̽��� �浹�ڽ�
	float x, y;						//�̽��� ��ǥ
	float angle;					//�̽��� �ޱ�
	float radius;					//������
	float speed;					//�ӵ�
	float fireX, fireY;				//�̽��� �߻���� ��ǥ
	bool fire;						//����?
	int currentFrameX, currentFrameY;
	int count;
	float power;
	int type;
};

//�������� ����ϴ� �Ѿ�
class FireBall : public gameNode
{
private:
	vector<tagFireBall> _vFireBall;
	vector<tagFireBall>::iterator _viFireBall;

	image* _fireBallImage;			//�̽��� �̹���
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

	//for A* �˰���
	FireBall();
	~FireBall();
};
