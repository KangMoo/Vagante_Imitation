#pragma once
#include "gameNode.h"

class Player;
class UI : public gameNode
{
private:
	bool _active;
	Player* _player;
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void draw();

	//설정자 접근자
	bool getActive() { return _active; }
	void setActive(bool active) { _active = active; }

	void setPlayerAddressLink(Player* player) { _player = player; }

	UI();
	~UI();
};

