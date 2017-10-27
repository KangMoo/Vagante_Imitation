#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "EnemyManager.h"
#include "Map.h"

UI::UI()
{
}


UI::~UI()
{
}

HRESULT UI::init()
{
	_active = false;
	for (int i = 0; i < 10; i++)
	{

	}
	for (int i = 0; i < 5; i++)
	{

	}
	tagItemInfo _item[5];
	tagCoinInfo _coin[255];
	return S_OK;
}
void UI::release()
{

}
void UI::update()
{

}
void UI::render()
{

}
void UI::draw()
{

}

void UI::addItemOnMap(tagItemInfo item)
{

}

void UI::addCoinOnMap(POINT coinPoint)
{
	//해당 POINT에 동전 스폰
}

void UI::showNumber(int num, COLORREF color)
{

}
