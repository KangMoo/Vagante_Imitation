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
	TYPE_WEAPON,
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

struct tagHitOutput
{
	float x, y;
	int damage;
	int alphaSource;
	LETTERFONT font;
};

class EnemyManager;
class Player;
class Map;


class UI : public gameNode
{
private: // �κ��丮 ����
	typedef vector<tagItem> vBag;
	typedef vector<tagItem>::iterator viBag;

	vBag _vBag;
	viBag _viBag;
private: //  Ÿ�� ��ġ ��� ����
	typedef vector<tagHitOutput> vHit;
	typedef vector<tagHitOutput>::iterator viHit;

	vHit _vHit;
	viHit _viHit;
private:
	tagDelay _delay;
	tagSave _save;
	RECT _menuRect;
	RECT _lvlRect;

private:
	int _count;
	int _currentHp;
	int _maxHp;
	int _menuNum;
	int _currentMoney;
	int _income;
	int _lvlNum;
	int _bagNum;
	int _statNum;
	int _skilNum;
	int _rankNum;
	int _lvlPoint;
	int _inputAlphaSource;
	int _plusAlphaSource;
	int _inputGuide;
	char* _tmp;
	bool _move;


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
	void render(POINT camera);
	void draw(POINT camera);
	void explanation();
	void setItemToBag(ITEMNAME name);

	void addImg();
	void keyControl();
	void rectMove();
	void setCoin(int coin);
	void repeatIndex(string keyName, int delay);
	void coinNumberMacro(NUMBERFONT font, float x, float y, int num);
	void hpTopNumberMacro(float x, float y, int num);
	void hpBottomNumberMacro(float x, float y, int num);
	void letterMacro(LETTERFONT font, float x, float y, char *str);
	void letterMacro(LETTERFONT font, float x, float y, char *str, int alpha);
	void setInputGuide();
	void hitOutput(float x, float y, int damage, LETTERFONT font);
	//�ʿ� ������ ����~
	void addItemOnMap(tagItemInfo item, POINT position);

	//�ʿ� �� ����~ (��ġ������ �����ϴ�~)
	void addCoinOnMap(POINT coinPoint); 

	//���� ǥ��
	void showNumber(int num, COLORREF color);

	//������ ������
	bool getActive() { return _active; }
	void setActive(bool active) { _active = active; setInputGuide(); }

	void setPlayerAddressLink(Player* player) { _player = player; }
	void setEnemyManagerAddressLink(EnemyManager* em) { _em = em; }
	void setMapAddressLink(Map* map) { _map = map; }

	UI();
	~UI();
};

