#pragma once
#include "gameNode.h"
#include "vaganteStructEnum.h"

#define SPACE 12

enum NUMBERFONT
{
	NUMBER_COIN_YELLOW,
	NUMBER_COIN_GREEN,
	NUMBER_COIN_RED,
	NUMBER_END
};

enum LETTERFONT
{
	LETTER_WHITE,
	LETTER_RED,
	LETTER_BLUE,
	LETTER_GRAY,
	LETTER_GREEN,
	LETTER_END
};

enum ITEMTYPE
{
	TYPE_GEAR,
	TYPE_POTION,
	TYPE_END
};

enum ITEMNAME
{
	NAME_SWORD,
	NAME_HEAL,
	NAME_END
};

struct tagDelay
{
	int coin;
	int menu;
};

struct tagItem
{
	image* img;
	ITEMTYPE type;
	ITEMNAME name;
	bool equip;
	int position;
	int alphaSource;

};

struct tagCoinInfo {
	image* img;		//���� �̹���
	POINT point;	//���� ��ǥ(�ʿ� ���� ��)
	RECT rc;		//���� ��Ʈ(�ʿ� ���� �� ���)
};

struct tagSave
{
	int position0;
	int position1;

};

class EnemyManager;
class Player;
class Map;


class UI : public gameNode
{
private:
	typedef vector<tagItem> vBag;
	typedef vector<tagItem>::iterator viBag;

	vBag _vBag;
	viBag _viBag;

private:
	tagDelay _delay;
	tagSave _save;
	RECT _menuRect;
	RECT _lvlRect;

private:
	int _count;
	int _currentHp;
	int _MaxHp;
	int _menuNum;
	int _currentMoney;
	int _income;
	int _lvlNum;
	int _bagNum;
	int _statNum;
	int _skilNum;
	int _rankNum;
	int _lvlPoint;
	char* _tmp;
	bool _move;
	int _inputAlphaSource;
	int _plusAlphaSource;


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
	void explanation();
	void setItemToBag(ITEMNAME name);
	void positionChange(int sour, int dest);

	void addImg();
	void keyControl();
	void rectMove();
	void setCoin(int coin);
	void repeatIndex(string keyName, int delay);
	void coinNumberMacro(NUMBERFONT font, float x, float y, int num);
	void letterMacro(LETTERFONT font, float x, float y, char *str);
	void letterMacro(LETTERFONT font, float x, float y, char *str, int alpha);


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

