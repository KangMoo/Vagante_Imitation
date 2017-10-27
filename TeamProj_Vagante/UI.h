#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

class EnemyManager;
class Player;
class Map;

/* 
!vaganteStructEnum.h���� ���������� ������ �� ��!
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

enum ITEMSTATE {
	ITEMSTATE_ONMAP,
	ITEMSTATE_INVENTORY,
	ITEMSTATE_EQUIP
};
struct tagItemInfo {
	image* bigImg;			//������ �̹���(ū��)
	image* smallImg;		//������ �̹���(������)
	POINT point;			//������ ��ǥ(�ʿ� ���� ��)
	RECT rc;				//������ ��Ʈ(�ʿ� ���� �� ���)
	tagStat itemstat;		//������ �ɷ�ġ
	int itemstate;			//���� �������� ����(��,�κ��丮,���������� Ȯ��)
};
*/
struct tagCoinInfo {
	image* img;		//���� �̹���
	POINT point;	//���� ��ǥ(�ʿ� ���� ��)
	RECT rc;		//���� ��Ʈ(�ʿ� ���� �� ���)
};
class UI : public gameNode
{
private:
	bool _active;
	Player* _player;
	EnemyManager* _em;
	Map* _map;
	tagItemInfo _item[5];
	tagCoinInfo _coin[100];

public:
	HRESULT init();
	void release();
	void update();
	void render();
	void draw();

	//�ʿ� ������ ����~
	void addItemOnMap(tagItemInfo item);

	//�ʿ� �� ����~ (��ġ������ �����ϴ�~)
	void addCoinOnMap(POINT coinPoint); 

	//���� ǥ��
	void showNumber(int num, COLORREF color);

	//������ ������
	bool getActive() { return _active; }
	void setActive(bool active) { _active = active; }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }

	UI();
	~UI();
};

