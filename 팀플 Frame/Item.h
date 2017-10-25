#pragma once
#include "gameNode.h"

struct tagItem {
	image* itemImage_big;
	image* itemImage_small;
	
};
class Item : public gameNode
{
private:
	
public:
	HRESULT init();
	void release();
	void update();
	void render();
	void draw();

	Item();
	~Item();
};

